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
	{ 0x480fe5b9, "__spi_register_driver" },
	{ 0xbaedff63, "_dev_info" },
	{ 0xdcb764ad, "memset" },
	{ 0x3b181693, "spi_sync" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x37f9546d, "spi_setup" },
	{ 0x26ec22df, "_dev_err" },
	{ 0x92893115, "driver_unregister" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("spi:w25qxx");
MODULE_ALIAS("of:N*T*Cwinbond,w25q32");
MODULE_ALIAS("of:N*T*Cwinbond,w25q32C*");

MODULE_INFO(srcversion, "7642FA6CD661345337CF631");
