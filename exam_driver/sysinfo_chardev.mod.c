#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xb7dc8e46, "module_put" },
	{ 0xf17d3809, "try_module_get" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x96b29254, "strncasecmp" },
	{ 0xdcb764ad, "memset" },
	{ 0x92997ed8, "_printk" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xc60d0620, "__num_online_cpus" },
	{ 0x15ba50a6, "jiffies" },
	{ 0x418c10ec, "__register_chrdev" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "046A98C0BEA97575E9C36CA");
