/*
 *My driver module which while allow me to activate and deactivate
 *the caps lock light on my keyboard
 *
 */

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/kd.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fcntl.h>



#define DRIVER_AUTHOR "Connor \"El Capitan\" Reeder"
#define DRIVER_LICENSE "GPL"
#define DRIVER_DESC "A driver kernel module which allows activation and deactivation of the num lock light"
#define DEVICE_NAME "numlight"
#define DRIVER_NAME "numdrv"
#define DEVICE_PATH "/dev/numlight"

#define SUCCESS 0
#define BUF_LEN 80

static int light_open(struct inode*, struct file*);
static int light_release(struct inode*, struct file*);
static ssize_t light_read(struct file* filp, char* buffer, size_t length,
		loff_t* offset);
static ssize_t light_write(struct file*, const char*, size_t, loff_t*);

/*
 *Global variables to be used only within this file
 */

static dev_t first;
static struct cdev c_dev;
static struct class* c1;
static int Light_Open = 0;



static int result;
static mm_segment_t oldfs;
static struct file* f;



//static char msg[BUF_LEN];
//static char* msg_Ptr;

static struct file_operations fops = {
	.read = light_read,
	.write = light_write,
	.open = light_open,
	.release = light_release
};

static int __init light_init(void) {
	if (alloc_chrdev_region(&first, 0, 1, DRIVER_NAME) < 0)
		return -1;
	if ((c1 = class_create(NULL, "chardrv")) == NULL) {
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
	f = filp_open("/sys/class/leds/input0::numlock/brightness",
			O_RDWR, 0666);

	return SUCCESS;
}

static void __exit light_exit(void) {
	/*
	 * Unregister the device
	 */
	cdev_del(&c_dev);
	device_destroy(c1, first);
	class_destroy(c1);
	unregister_chrdev_region(first,1);
	printk(KERN_INFO "Just unregistered the num lock light\n");
}

module_init(light_init);
module_exit(light_exit);




/*
 *My file operation functions
 *to be called when a process tries to do something with my device file
 */

static int light_open(struct inode* inode, struct file* file){
	if(Light_Open)
		return -EBUSY;

	Light_Open++;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static int light_release(struct inode* inode, struct file* file) {
	Light_Open--;
	module_put(THIS_MODULE);
	return SUCCESS;
}
static ssize_t light_read(struct file* filp, char* buffer, size_t length,
		loff_t* offset){

	char kbuf[2] = {0,'\n'};

	if (*offset > 0)
		return 0;


    	oldfs = get_fs();
	set_fs(KERNEL_DS);
	f->f_pos = 0;
	f->f_op->read(f,kbuf, 1, &f->f_pos);
	set_fs(oldfs);


	if (copy_to_user((void __user *)buffer, (char*) kbuf, 2))
		return -EFAULT;

	*offset = *offset + 1;
	//printk(KERN_INFO "Just called light_read %d %d\n", kbuf[0], kbuf[1]);
	return 2;
}
static ssize_t light_write(struct file* file, const char* buffer, size_t length, loff_t* offset){

	const char kbuf[5];

	if (copy_from_user((char*) kbuf, (const char __user *)buffer, length < 5 ? length : 5))
		return -EFAULT;


    	oldfs = get_fs();
	set_fs(KERNEL_DS);
	f->f_pos = 0;
	if (kbuf[0] != '0')
		result = f->f_op->write(f, "3\n", 2, &f->f_pos);
	else
		result = f->f_op->write(f, "0\n", 2, &f->f_pos);
	set_fs(oldfs);

	
	//printk(KERN_INFO "Just called light_write %s %d %d %d %d %d\n", kbuf,kbuf[0], kbuf[1], kbuf[2], kbuf[3], kbuf[4]);
	return length;
}
MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
