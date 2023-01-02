#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0x64491f21, "module_layout" },
	{ 0xd581696b, "put_disk" },
	{ 0xb5a459dc, "unregister_blkdev" },
	{ 0x2197e28a, "blk_cleanup_queue" },
	{ 0x999e8297, "vfree" },
	{ 0x159c8b23, "del_gendisk" },
	{ 0x944c49b, "device_add_disk" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0xed88fb0f, "blk_mq_init_sq_queue" },
	{ 0x71a50dbc, "register_blkdev" },
	{ 0xc5850110, "printk" },
	{ 0xa4a6b890, "__alloc_disk_node" },
	{ 0x56470118, "__warn_printk" },
	{ 0xe252790, "blk_mq_end_request" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0x97651e6c, "vmemmap_base" },
	{ 0x69acdf38, "memcpy" },
	{ 0xf6b19364, "blk_mq_start_request" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "3B23F7A7D33C93E94254547");
