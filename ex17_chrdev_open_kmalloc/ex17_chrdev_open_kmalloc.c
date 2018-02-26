#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#define GLOBALSIZE 32

static unsigned major = 500;
static unsigned minor =0;
static unsigned count = 1;
static char * name = "test_dev";
static dev_t devno;

static struct cdev cdev;

static struct scull_device{
	char mem[GLOBALSIZE];
};

static int goldage_driver_open(struct inode * inode,struct file * filp){
	struct scull_device * dev = NULL;
	/* 1. Use kmalloc when open. */
	dev = (struct scull_device*)kmalloc(sizeof(*dev),GFP_KERNEL);

	filp->private_data = dev;
	
	memset(dev->mem,0,sizeof(char)*GLOBALSIZE);
	memcpy(dev->mem,"abc",3);

	printk("goldage_driver_open.\n");
	return 0;
}

static int goldage_driver_release(struct inode * inode,struct file * filp){
	struct scull_device * dev = filp->private_data;
	printk("dev->mem : %s\n",dev->mem);

	/* 2. Use kfree. */
	kfree(dev);
	printk("goldage_driver_release.\n");
	return 0;
}
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = goldage_driver_open,
	.release = goldage_driver_release,
};

static int goldage_driver_init(void){
	int ret = 0 ;

	devno = MKDEV(major,minor);
	if(register_chrdev_region(devno , count , name)){
		printk("register_chrdev_region failed.\n");
		return -1;
	}

	cdev_init(&cdev , &fops);
	ret = cdev_add(&cdev , devno , count);
	if(ret){
		printk("cdev_add failed.\n");
		goto err0;
	}

	printk("devno : %d\tmajor : %d\tminor : %d\n",devno,MAJOR(devno),MINOR(devno));
	printk("goldage_driver_init.\n");
	return 0;

err0:
	unregister_chrdev_region(devno,count);
}

static void goldage_driver_exit(void){
	cdev_del(&cdev);
	unregister_chrdev_region(devno,count);
	printk("Bye , kernel.\n");
}

module_init(goldage_driver_init);
module_exit(goldage_driver_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");
