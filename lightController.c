/*
 *My driver module which while allow me to activate and deactivate
 *the caps light and num light on my keyboard
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
#define DRIVER_DESC "A driver kernel module which allows activation and deactivation of the caps light and num light"
#define DRIVER_NAME "lightcontroller"
#define DEVICE_CLASS "lights"

#define SUCCESS 0

static int light_open(struct inode*, struct file*);
static int light_release(struct inode*, struct file*);
static ssize_t light_read(struct file* filp, char* buffer, size_t length,
		loff_t* offset);
static ssize_t light_write(struct file*, const char*, size_t, loff_t*);

/*
 *Global variables to be used only within this file
 */

static dev_t devices[2];
static struct cdev c_dev_cap;
static struct cdev c_dev_num;
static struct class* c1;
static int Light_Open = 0;



static int result;
static mm_segment_t oldfs;
static struct file* capsFile;
static struct file* numFile;
static struct file* f;




static struct file_operations fops = {
	.read = light_read,
	.write = light_write,
	.open = light_open,
	.release = light_release
};

static int __init light_init(void) {
	if (alloc_chrdev_region(&devices[0], 0, 1, DRIVER_NAME) < 0)
		return -1;
	if (alloc_chrdev_region(&devices[1], 0, 1, DRIVER_NAME) < 0){
		unregister_chrdev_region(devices[0], 1);
		return -1;
	}
	if ((c1 = class_create(THIS_MODULE, DEVICE_CLASS)) == NULL) {
		unregister_chrdev_region(devices[0], 1);
		return -1;
	}
	if (device_create(c1, NULL, devices[0], NULL, "capslight") == NULL) {
		class_destroy(c1);
		unregister_chrdev_region(devices[0], 1);
		unregister_chrdev_region(devices[1], 1);
		return -1;
	}
	if (device_create(c1, NULL, devices[1], NULL, "numlight") == NULL) {
		class_destroy(c1);
		device_destroy(c1, devices[1]);
		unregister_chrdev_region(devices[0], 1);
		unregister_chrdev_region(devices[1], 1);
		return -1;
	}
	cdev_init(&c_dev_cap, &fops);
	if(cdev_add(&c_dev_cap, devices[0], 1) == -1) {
		device_destroy(c1, devices[0]);
		device_destroy(c1, devices[1]);
		class_destroy(c1);
		unregister_chrdev_region(devices[0], 2);
		return -1;
	}
	cdev_init(&c_dev_num, &fops);
	if(cdev_add(&c_dev_num, devices[1], 1) == -1) {
		device_destroy(c1, devices[0]);
		device_destroy(c1, devices[1]);
		class_destroy(c1);
		unregister_chrdev_region(devices[0], 2);
		return -1;
	}
	capsFile = filp_open("/sys/class/leds/input0::capslock/brightness",
			O_RDWR, 0666);

	numFile = filp_open("/sys/class/leds/input0::numlock/brightness",
			O_RDWR, 0666);
	f = capsFile;
	return SUCCESS;
}

static void __exit light_exit(void) {
	/*
	 * Unregister the device
	 */
	cdev_del(&c_dev_cap);
	cdev_del(&c_dev_num);
	device_destroy(c1, devices[0]);
	device_destroy(c1, devices[1]);
	class_destroy(c1);
	unregister_chrdev_region(devices[0],1);
	unregister_chrdev_region(devices[1],1);
	printk(KERN_INFO "Just unregistered the caps and num lights\n");
}

module_init(light_init);
module_exit(light_exit);




/*
 *My file operation functions
 *to be called when a process tries to do something with my device file
 */

static int light_open(struct inode* inode, struct file* file){

	printk(KERN_INFO "Bootup capsFile: %p\n", capsFile);
	printk(KERN_INFO "Bootup numFile: %p\n", numFile);
	Light_Open++;
	try_module_get(THIS_MODULE);

	if (inode->i_cdev->dev == devices[0]) {
		//printk(KERN_INFO "Caps Light opened!\n");
		if (!capsFile)
			return -1;
	}
	//num light       
	else if (inode->i_cdev->dev == devices[1]){
		//printk(KERN_INFO "Num light opened!\n");
		if (!numFile)
			return -1;
	} else {
		printk(KERN_INFO "Some other light is being written to!\n");
		return -1;
	}

	return SUCCESS;
}

static int light_release(struct inode* inode, struct file* file) {
	Light_Open--;
	module_put(THIS_MODULE);
	return SUCCESS;
}
static ssize_t light_read(struct file* file, char* buffer, size_t length,
		loff_t* offset){

	char kbuf[2] = {0,'\n'};

	if (*offset > 0)
		return 0;

	//capslight
	if (file->f_inode->i_cdev->dev == devices[0]) {
		f = capsFile;
	}
	//num light       
	else if (file->f_inode->i_cdev->dev == devices[1]){
		f = numFile;
	} 

    	oldfs = get_fs();
	set_fs(KERNEL_DS);
	f->f_pos = 0;
	f->f_op->read(f,kbuf, 1, &f->f_pos);
	set_fs(oldfs);


	if (copy_to_user((void __user *)buffer, (char*) kbuf, 2))
		return -EFAULT;

	*offset = *offset + 1;
	return 2;
}
static ssize_t light_write(struct file* file, const char* buffer, size_t length, loff_t* offset){

	const char kbuf[5];
	//int i;
	if (copy_from_user((char*) kbuf, (const char __user *)buffer, length < 5 ? length : 5))
		return -EFAULT;
	//capslight
	if (file->f_inode->i_cdev->dev == devices[0]) {
		f = capsFile;
	}
	//num light       
	else if (file->f_inode->i_cdev->dev == devices[1]){
		f = numFile;
	} 
    	oldfs = get_fs();
	set_fs(KERNEL_DS);
	f->f_pos = 0;
	if (kbuf[0] != '0')
		result = f->f_op->write(f, "3\n", 2, &f->f_pos);
	else
		result = f->f_op->write(f, "0\n", 2, &f->f_pos);
	set_fs(oldfs);
	return length;
}
MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
