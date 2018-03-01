#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

static unsigned baseminor = 0;
static unsigned count = 1;
const char * name = "test_dev";
static dev_t devno;

static struct cdev cdev;
/* 1. Define class. */
struct class * goldage_class;

static int goldage_driver_open(struct inode * inode,struct file * filp){
	printk("goldage_driver_open.\n");
	return 0;
}

static int goldage_driver_release(struct inode * inode,struct file * filp){
	printk("goldage_driver_release.\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = goldage_driver_open,
	.release = goldage_driver_release,
};

static int goldage_driver_init(void)
{
	int ret = 0;

	if(alloc_chrdev_region(&devno,baseminor,count,name)){
		printk("alloc_chrdev_region failed.\n");
		return -1;
	}

	cdev_init(&cdev,&fops);
	ret = cdev_add(&cdev,devno,count);
	if(ret){
		printk("cdev_add failed.err:%d.\n",ret);
		goto err0;
	}

	/* 2. Create class. */
	goldage_class = class_create(THIS_MODULE,"goldage_class");
	device_create(goldage_class,NULL,devno,NULL,"ex");

	printk("devno:%d , major: %d , minor : %d.\n",devno,MAJOR(devno),MINOR(devno));
	printk("goldage_driver_init.\n");
	return 0;
err0:
	unregister_chrdev_region(devno,count);
	return 0;
}

static void goldage_driver_exit(void)
{
	cdev_del(&cdev);
	unregister_chrdev_region(devno,count);

	device_destroy(goldage_class,devno);
	class_destroy(goldage_class);

	printk("Bye , kernel.\n");
}

module_init(goldage_driver_init);
module_exit(goldage_driver_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");
