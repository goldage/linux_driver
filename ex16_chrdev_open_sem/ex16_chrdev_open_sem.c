#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

static unsigned major = 500;
static unsigned minor =0;
static unsigned count = 1;
static char * name = "test_dev";
static dev_t devno;

static struct cdev cdev;

/* 1. Define semaphore variable. */
static struct semaphore sem;

static int goldage_driver_open(struct inode * inode,struct file * filp){
	/* 3. Down semaphore variable. */
	down_interruptible(&sem);

	printk("goldage_driver_open.\n");
	return 0;
}

static int goldage_driver_release(struct inode * inode,struct file * filp){
	/* 4. Up semaphore variable. */
	up(&sem);

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

	/* 2. Initialize the semaphore variable. */
	sema_init(&sem,1);

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
