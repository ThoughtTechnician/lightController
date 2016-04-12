/*
 *hello-mod.c - This is my first linux kernel module!!!
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>


int init_module(void)
{
	printk(KERN_INFO "Hello World, my name is Kernor Reedule!!!\n");

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world, that's all folks!!!\n");
}
