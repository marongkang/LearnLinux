#include <linux/blk-mq.h>
#include <linux/blkdev.h>
#include <linux/genhd.h>
#include <linux/hdreg.h>
#include <linux/init.h>
#include <linux/module.h>

#define BLKSIZE 256 * 1024 * 1024
#define BLKNAME "mydisk"
struct gendisk* disk;
int major;
struct request_queue* queue;
char* dev_addr;
struct blk_mq_tag_set tag;

int mydisk_open(struct block_device* blkdev, fmode_t mode) {
    return 0;
}
int mydisk_getgeo(struct block_device* blkdev,struct hd_geometry* hd) {

    //     struct hd_geometry {
    //       unsigned char heads; //磁头的个数
    //       unsigned char sectors; //扇区个数
    //       unsigned short cylinders; //磁道的个数
    //       unsigned long start;   //0
    // };

    hd->heads = 4;
    hd->cylinders = 16;
    hd->sectors = BLKSIZE / hd->heads / hd->cylinders / 512;
    return 0;
}
void mydisk_close(struct gendisk* dk, fmode_t mode) {}

struct block_device_operations fops = {
        .open = mydisk_open,
        .getgeo = mydisk_getgeo,
        .release = mydisk_close,
};
blk_status_t disk_queue_rq(struct blk_mq_hw_ctx* ctx,const struct blk_mq_queue_data* data) {
    blk_status_t status = BLK_STS_OK;
    struct request* rq = data->rq;
    struct bio_vec bvec; // 循环取到的成员
    struct req_iterator iter; // request_iterator
    loff_t pos = blk_rq_pos(rq) << SECTOR_SHIFT;

    // deal request
    blk_mq_start_request(rq);

    rq_for_each_segment(bvec, rq, iter) {
        unsigned long b_len = bvec.bv_len;

        //内核的地址
        void* b_buf = page_address(bvec.bv_page) + bvec.bv_offset;

        //校验越界
        if ((pos + b_len) > BLKSIZE)
            b_len = (unsigned long)(BLKSIZE - pos);

        if (rq_data_dir(rq)) // WRITE

            memcpy(dev_addr+pos, b_buf, b_len);
        else // READ

            memcpy(b_buf,dev_addr+pos, b_len);

        pos += b_len;
    }

    blk_mq_end_request(rq, status);

    return status;
}
struct blk_mq_ops qops = {
        .queue_rq = disk_queue_rq,
};
static int __init mydisk_init(void) {
    // alloc disk memory
    disk = alloc_disk(4);
    if (disk == NULL) {
        printk("alloc disk memory error\n");
        return -ENOMEM;
    }
    // init
    major = register_blkdev(0, BLKNAME);
    if (major < 0) {
        printk("register blk device error\n");
        return major;
    }
    queue = blk_mq_init_sq_queue(&tag, &qops, 2, BLK_MQ_F_SHOULD_MERGE);
    if (IS_ERR(queue)) {
        printk("blk queue get error\n");
        return PTR_ERR(queue);
    }
    disk->major = major;
    disk->first_minor = 0;
    strcpy(disk->disk_name, BLKNAME);
    set_capacity(disk, BLKSIZE / 512); //将磁盘默认先全部给part0
    disk->fops = &fops;
    disk->queue = queue;

    // alloc memory
    dev_addr = vmalloc(BLKSIZE);
    if (dev_addr == NULL) {
        printk("alloc dev memory error\n");
        return -ENOMEM;
    }

    add_disk(disk);

    return 0;
}
static void __exit mydisk_exit(void)
{
    del_gendisk(disk);
    vfree(dev_addr);
    blk_cleanup_queue(queue);
    unregister_blkdev(major, BLKNAME);
    put_disk(disk);
}
module_init(mydisk_init);
module_exit(mydisk_exit);

MODULE_LICENSE("GPL");
