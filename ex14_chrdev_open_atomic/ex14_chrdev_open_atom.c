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

/* 1. Define atom variable. */
static atomic_t v;

static int goldage_driver_open(struct inode * inode,struct file * filp){
	/*3. Dec atom variable and test. */
	if(atomic_dec_and_test(&v)){

	}else{
		/* Although fail, atom has been dec. */
		atomic_inc(&v);
		printk("goldage_driver_open failed.\n");
		printk("v:%d\n",v);
		return -EBUSY;
	}
	printk("goldage_driver_open.\n");
	printk("v:%d\n",v);
	return 0;
}

static int goldage_driver_release(struct inode * inode,struct file * filp){
	/*4. Inc atom variable. */
	atomic_inc(&v);
	printk("goldage_driver_release.\n");
	printk("v:%d\n",v);
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

	/*2. Initialize atom variable. */
	atomic_set(&v,1);

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
