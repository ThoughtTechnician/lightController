/*
 * second-mod.c - Getting a little fancier with the kernel modules
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define DRIVER_AUTHOR "Connor \"El Capitan\" Reeder"
#define DRIVER_DESC "Getting a little fancier with the kernel modules"


static int init_second(void) {
	printk(KERN_INFO "Second module getting fancy\n");
	return 0;
}

static void exit_second(void) {
	printk(KERN_INFO "Second module is exiting\n");
}

module_init(init_second);
module_exit(exit_second);


/*
 *Module metadata
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

MODULE_SUPPORTED_DEVICE("connorsdevice");
