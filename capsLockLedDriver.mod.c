#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x6bad2f62, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x582d7203, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0xc42059e, __VMLINUX_SYMBOL_STR(filp_open) },
	{ 0x4d45a1d5, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x8451c988, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x3e4f8420, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x9939e383, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0xaddb113b, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x4234217a, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x3356b90b, __VMLINUX_SYMBOL_STR(cpu_tss) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x301c4a2e, __VMLINUX_SYMBOL_STR(try_module_get) },
	{ 0xbd881263, __VMLINUX_SYMBOL_STR(module_put) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

