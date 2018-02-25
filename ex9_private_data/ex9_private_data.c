#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define GLOBALSIZE 128

/*1. Define device number. */
static unsigned major = 500;
static unsigned minor = 0;
static unsigned count = 1;
const char * name = "test_dev";
static dev_t devno;

/*2. Define cdev structure. */
static struct scull_device{
	char mem[GLOBALSIZE];
	int len;
	struct cdev cdev;
}scull;

static int goldage_driver_open(struct inode * inode , struct file * filp)
{
	/*8. Define private data. */

	/* Usage 1. */
	//filp->private_data = &scull;
	/* Usage 2.*/
	filp->private_data = container_of(inode->i_cdev , struct scull_device , cdev);
	/* A question: what's the 3rd parameter usage. */
	printk("goldage_driver_open.\n");
	return 0;
}

static int goldage_driver_release(struct inode * inode , struct file * filp)
{
	/*9. Use private data to transmit. */
	struct scull_device * dev = filp->private_data;
	printk("scull_len:%d\n",dev->len);
	printk("goldage_driver_release.\n");
	return 0;
}

/*7. Define driver-method. */
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = goldage_driver_open,
	.release = goldage_driver_release,
};

static int goldage_driver_init(void)
{
	int ret = 0;

	/*3. Acquire device number. */
	devno = MKDEV(major,minor);
	/*4. Register device number. */
	if(register_chrdev_region(devno , count , name)){
		printk("register_chrdev_region failed.\n");
		return -1;
	}
	/*5. Initialize cdev*/
	cdev_init(&scull.cdev,&fops);
	/*6. Add cdev to kernel. */
	ret = cdev_add(&scull.cdev,devno,count);
	if(ret){
		printk("cdev_add failed,err:%d\n",ret);
		goto err0;
	}

	scull.len = 4;

	printk("devno:%d,major:%d,minor:%d\n",devno,MAJOR(devno),MINOR(devno));
	printk("goldage_driver_init.\n");
	return 0;

err0:
	unregister_chrdev_region(devno,count);
	return 0;
}

static void goldage_driver_exit(void)
{
	cdev_del(&scull.cdev);
	unregister_chrdev_region(devno,count);
	printk("Bye , kernel.\n");
}

module_init(goldage_driver_init);
module_exit(goldage_driver_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");
