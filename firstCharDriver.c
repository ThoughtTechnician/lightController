/*
 *firstCharDriver.c - My first kernel module that acts 
 *			as a character device driver
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>

/*
 *Prototypes 
 */

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode*, struct file*);
static int device_release(struct inode*, struct file*);
static ssize_t device_read(struct file*, char*, size_t, loff_t*);
static ssize_t device_write(struct file*, const char*, size_t, loff_t*);

#define DRIVER_AUTHOR "Connor \"El Capitan\" Reeder"
#define DRIVER_DESC "Getting a little fancier with the kernel modules"
#define DRIVER_LICENSE "GPL"

#define SUCCESS 0
#define DEVICE_NAME "connorsdev"
#define BUF_LEN 80

/*
 *Global variables to be used only within this file
 *
 */

static dev_t first;
static struct cdev c_dev;
static struct class *c1;
static int Device_Open = 0;

static char msg[BUF_LEN];
static char* msg_Ptr;

static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};


static int __init my_init(void) {
	if (alloc_chrdev_region(&first, 0, 1, "connorsdrv") < 0)
		return -1;
	if ((c1 = class_create(THIS_MODULE, "chardrv")) == NULL) {
		unregister_chrdev_region(first, 1);
		return -1;
	}
	if (device_create(c1, NULL, first, NULL, DEVICE_NAME) == NULL) {
		class_destroy(c1);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	cdev_init(&c_dev, &fops);
	if(cdev_add(&c_dev, first, 1) == -1) {
		device_destroy(c1, first);
		class_destroy(c1);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	return SUCCESS;
}

static void __exit my_exit(void) {
	/*
	 * Unregister the device
	 */
	cdev_del(&c_dev);
	device_destroy(c1, first);
	class_destroy(c1);
	unregister_chrdev_region(first,1);
	printk(KERN_INFO "Connor just called unregister_chrdev\n");
}	

module_init(my_init);
module_exit(my_exit);

/*
 *
 *My functions
 *
 */

/* 
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode* inode, struct file* file) {
	static int counter = 0;

	if(Device_Open)
		return -EBUSY;

	Device_Open++;
	sprintf(msg, "I already told you %d times Hello world!\n", counter++);
	msg_Ptr = msg;
	try_module_get(THIS_MODULE); //Not sure I understand this

	return SUCCESS;
}


/* 
 * Called when a process closes the device file.
 */

static int device_release(struct inode* inode, struct file* file) {
	Device_Open--;

	module_put(THIS_MODULE); //Not sure I understand this

	return 0;

}
/* 
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file* filp, char* buffer, size_t length,
		loff_t* offset) {
	int bytes_read = 0;
	if (*msg_Ptr == 0)
		return 0;

	while(length && *msg_Ptr) {
		put_user(*(msg_Ptr++), buffer++);
		length--;
		bytes_read++;
	}

	return bytes_read;
}

/*  
 * Called when a process writes to dev file: echo "hi" > /dev/hello 
 */
static ssize_t device_write(struct file* filp, const char* buff, size_t len,
		loff_t* off) {
	printk(KERN_ALERT "Sorry, this operation is not supported by connor\n");
	return -EINVAL;
}

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

