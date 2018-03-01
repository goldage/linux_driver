#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/device.h>

#define GLOBALSIZE 128

static unsigned baseminor = 0;
static unsigned count = 1;
const char * name = "test_dev";
static dev_t devno;

static struct scull_device{
	struct cdev cdev;
	unsigned int sec;

	struct class * goldage_class;
	/* 1.Define timer list.*/
	struct timer_list timer;
}scull;

static int goldage_driver_open(struct inode * inode,struct file * filp)
{
	filp->private_data = container_of(inode->i_cdev,struct scull_device,cdev);
	printk("goldage_driver_open.\n");
	return 0;
}

static int goldage_driver_release(struct inode * inode, struct file * filp)
{
	struct scull_device * dev = filp->private_data;
	printk("goldage_driver_release.\n");
	return 0;
}

static ssize_t goldage_driver_read(struct file * filp,char __user* buf,size_t cnt,loff_t * lops)
{
	put_user(scull.sec,(unsigned int*)buf);
	return sizeof(unsigned int);
}

/* 4. Define timer func. */
static void goldage_driver_timer(unsigned long args)
{
	/* 5. Use mod_timer. */
	mod_timer(&scull.timer,jiffies + HZ);
	scull.sec += 1;
	printk("jiffies:%ld.\n",jiffies);
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = goldage_driver_open,
	.release = goldage_driver_release,
	.read = goldage_driver_read,
};

static int goldage_driver_init(void){
	int ret = 0;

	if(alloc_chrdev_region(&devno,baseminor,count,name)){
		printk("alloc_chrdev_region failed.\n");
		return -1;
	}

	cdev_init(&scull.cdev,&fops);
	ret = cdev_add(&scull.cdev,devno,count);
	if(ret){
		printk("cdev_add failed.\n");
		goto err0;
	}

	scull.goldage_class = class_create(THIS_MODULE,"test_class");
	device_create(scull.goldage_class,NULL,devno,NULL,"test_dev");

	/* 2. Initialize timer_list. */
	init_timer(&scull.timer);
	scull.timer.function = goldage_driver_timer;
	scull.timer.expires = jiffies + HZ;

	/* 3. Add timer to kernel.*/
	add_timer(&scull.timer);

	printk("devno : %d,major : %d, minor : %d.\n",devno,MAJOR(devno),MINOR(devno));
	printk("goldage_driver_init.\n");
	
	return 0;

err0:
	unregister_chrdev_region(devno,count);
	return 0;
}

static void goldage_driver_exit(void)
{
	del_timer(&scull.timer);
	cdev_del(&scull.cdev);
	unregister_chrdev_region(devno,count);

	device_destroy(scull.goldage_class,devno);
	class_destroy(scull.goldage_class);
}

module_init(goldage_driver_init);
module_exit(goldage_driver_exit)
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");
