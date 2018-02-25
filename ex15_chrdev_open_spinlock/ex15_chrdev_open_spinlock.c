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

/* 1. Define spinlock variable. */
static spinlock_t lock;
static int src_cnt;//Define src count.

static int goldage_driver_open(struct inode * inode,struct file * filp){
#if 0
	spin_lock(&lock);
#endif
	spin_lock(&lock);
	src_cnt += 1;
	spin_unlock(&lock);

	printk("goldage_driver_open.\n");
	return 0;
}

static int goldage_driver_release(struct inode * inode,struct file * filp){
#if 0
	spin_unlock(&lock);
#endif

	spin_lock(&lock);
	src_cnt -= 1;
	spin_unlock(&lock);

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

	/*2. Initialize spinlock variable. */
	spin_lock_init(&lock);
	src_cnt=0;//Initialize src count.

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
