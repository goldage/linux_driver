#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

/* 1. Define device number. */
static unsigned major = 500;
static unsigned minor =0;
static unsigned count = 1;
static char * name = "test_dev";

static dev_t devno;

/* 2. Define cdev structure. */
static struct cdev cdev;

static int goldage_driver_open(struct inode * inode,struct file * filp){
	printk("goldage_driver_open.\n");
	return 0;
}

static int goldage_driver_release(struct inode * inode,struct file * filp){
	printk("goldage_driver_release.\n");
	return 0;
}
/* 7. Define driver-method */
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = goldage_driver_open,
	.release = goldage_driver_release,
};

static int goldage_driver_init(void){
	int ret = 0 ;

	/* 3. Acquire device number.*/
	devno = MKDEV(major,minor);
	/* 4. Register device number. */
	if(register_chrdev_region(devno , count , name)){
		printk("register_chrdev_region failed.\n");
		return -1;
	}

	/* 5. Initialize cdev. */
	cdev_init(&cdev , &fops);
	/* 6. Add cdev to kernel. */
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
	/* 8. Release resource. */
	cdev_del(&cdev);
	unregister_chrdev_region(devno,count);
	printk("Bye , kernel.\n");
}

module_init(goldage_driver_init);
module_exit(goldage_driver_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");
