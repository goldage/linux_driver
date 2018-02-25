#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

/* 1. Define device number */
static unsigned major = 500;
static unsigned minor = 0;
static unsigned count = 1;
const char * name = "test_dev";

static dev_t devno;

/* 2. Define cdev structure */
static struct cdev cdev;

static struct file_operations fops = {
	.owner = THIS_MODULE,
};

static int goldage_driver_init(void){
	int ret = 0;

	/* 3. Use the static way to acquire device number */
	devno = MKDEV(major,minor);

	/* 4. register device number */
	if(register_chrdev_region(devno,count,name)){
		printk("register_chrdev_region failed.\n");
		goto err0;
	}

	/* 5. Initialize cdev */
	cdev_init(&cdev,&fops);

	/* 6. Add the cdev to the kernel */
	ret = cdev_add(&cdev,devno,count);
	if(ret){
		printk("cdev_add failed.\n");
		goto err1;
	}

	printk("goldage_driver_init.\n");
	printk("devno : %d\tmajor : %d\tminor : %d\n",devno,MAJOR(devno),MINOR(minor));

	return 0;

err1:
	unregister_chrdev_region(devno,count);
err0:
	return -1;
}

static void goldage_driver_exit(void){
	/* 7. Release  resource */
	cdev_del(&cdev);
	unregister_chrdev_region(devno,count);
	printk("Bye , kernel.\n");
}

module_init(goldage_driver_init);
module_exit(goldage_driver_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");
