#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/sched.h>

#define GLOBALSIZE 128

/*1. Define device number. */
static unsigned major = 500;
static unsigned minor = 0;
static unsigned count = 1;
const char * name = "test_dev";
static dev_t devno;

/*2. Define cdev structure. */
static struct scull_device {
	char mem[GLOBALSIZE];
	int len;
	struct cdev cdev;
	/* 10.Define wait queue header. */
	wait_queue_head_t rq,wq;
	/* 11.Define semaphore variable to prevent competitive state. */
	struct semaphore sem;
}scull;

static int goldage_driver_open(struct inode * inode,struct file * filp)
{
	/*8. Define private data.*/
	filp->private_data = container_of(inode->i_cdev,struct scull_device,cdev);
	printk("goldage_driver_open.\n");
	return 0;
}

static int goldage_driver_release(struct inode * inode,struct file * filp)
{
	/*9.Use private data to transmit. */
	struct scull_device * dev = filp->private_data;

	printk("scull.len:%d.\n",dev->len);
	printk("goldage_driver_release.\n");
	return 0;
}

static ssize_t goldage_driver_read(struct file * filp,char __user * buf,size_t cnt,loff_t * lops)
{
	struct scull_device* dev = filp->private_data;
	
	if(cnt < 0)return -EINVAL;
	
	/* 15. Use semaphore variable to prevent competitive state. */
	down_interruptible(&scull.sem);

	/* 16. Use wait queue to block. */
	if(dev->len == 0){
		up(&scull.sem);
		if(filp->f_flags & O_NONBLOCK){
			/* NONBLOCK */
			printk("Can not read: busy.\n");
			return -EAGAIN;
		}else{
			/* BLOCK */
			//msleep(500);
			wait_event_interruptible(scull.rq,(dev->len != 0));
		}
		down_interruptible(&scull.sem);
	}

	if(cnt > dev->len)cnt = dev->len;

	if(!copy_to_user(buf,dev->mem,cnt)){
		strcpy(dev->mem,dev->mem + cnt);
		dev->len -= cnt ;
	}else{
		printk("copt_to_user failed.\n");
		return -EINVAL;
	}

	up(&scull.sem);
	wake_up_interruptible(&scull.wq);
	return cnt;
}

static ssize_t goldage_driver_write(struct file * filp,const char __user * buf,size_t cnt,loff_t * lops)
{
	struct scull_device* dev = filp->private_data;

	if(cnt < 0)return -EINVAL;

	down_interruptible(&scull.sem);

	if(dev->len == GLOBALSIZE){
		up(&scull.sem);
		/* NONBLOCK */
		if(filp->f_flags & O_NONBLOCK){
			printk("Can not write : busy.\n");
			return -EAGAIN;
		}else{
			/* BLOCK */
			//msleep(500);
			wait_event_interruptible(scull.wq,(dev->len != GLOBALSIZE));
		}
		down_interruptible(&scull.sem);
	}

	if(cnt > GLOBALSIZE - dev->len)cnt = GLOBALSIZE - dev->len;

	if(copy_from_user(dev->mem + dev->len,buf,cnt)){
		printk("copy_from_user failed.\n");
		return -EFAULT;
	}else{
		dev->len += cnt;
	}

	up(&scull.sem);
	wake_up_interruptible(&scull.rq);
	printk("dev->mem:%s.\n",dev->mem);
	return cnt;
}

/*Define driver-method. */
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open  = goldage_driver_open, 
	.release = goldage_driver_release,
	.read = goldage_driver_read,
	.write = goldage_driver_write,
};

static int goldage_driver_init(void)
{
	int ret = 0;
	
	/*3. Acquire device number. */
	devno = MKDEV(major,minor);
	/*4. Register device number. */
	if(register_chrdev_region(devno,count,name)){
		printk("register_chrdev_region failed.\n");
		return -1;
	}
	/*5. Initialize cdev. */
	cdev_init(&scull.cdev,&fops);
	/*6. Add cdev to kernel. */
	ret = cdev_add(&scull.cdev,devno,count);
	if(ret){
		printk("cdev_add failed.\n");
		goto err0;
	}

	memset(scull.mem,0,sizeof(scull.mem));
	scull.len = 0;

	/* 12.Initialize waitqueue header. */
	init_waitqueue_head(&scull.rq);
	init_waitqueue_head(&scull.wq);

	/* 13.Initialize semaphore variable. */
	sema_init(&scull.sem,1);

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
