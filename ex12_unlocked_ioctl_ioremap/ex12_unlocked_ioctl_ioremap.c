#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <asm/io.h>
#include "goldage_ioctl.h"

#define GLOBALSIZE 128

/*0. Define physical address. */
#define LED_BASEADDR 0X11000000
#define LED_CONOFFSET 0X0C40
#define LED_DATOFFSET 0X0C44

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
	void __iomem* baseaddr;
	void __iomem* conaddr;
	void __iomem* dataddr;
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

	if(cnt > dev->len)cnt = dev->len;

	if(!copy_to_user(buf,dev->mem,cnt)){
		strcpy(dev->mem,dev->mem + cnt);
		dev->len -= cnt ;
	}else{
		printk("copt_to_user failed.\n");
		return -EINVAL;
	}

	return cnt;
}

static ssize_t goldage_driver_write(struct file * filp,const char __user * buf,size_t cnt,loff_t * lops)
{
	struct scull_device* dev = filp->private_data;

	if(cnt < 0)return -EINVAL;

	if(cnt > GLOBALSIZE - dev->len)cnt = GLOBALSIZE - dev->len;

	if(copy_from_user(dev->mem + dev->len,buf,cnt)){
		printk("copy_from_user failed.\n");
		return -EFAULT;
	}else{
		dev->len += cnt;
	}

	printk("dev->mem:%s.\n",dev->mem);
	return cnt;
}

static long goldage_unlocked_ioctl(struct file * filp,unsigned int cmd,unsigned long args)
{
	if(_IOC_TYPE(cmd) != MAGIC_TYPE) return -EINVAL;

	switch(_IOC_NR(cmd)){
		case 0:{
			writel(readl(scull.dataddr) | (0x1 << 7),scull.dataddr);	
			printk("LED_ON.\n");
			break;
		}
		case 1:{
			writel(readl(scull.dataddr) & ~(0x1 << 7),scull.dataddr);
			printk("LED_OFF.\n");
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

/*10. Define driver-method. */
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open  = goldage_driver_open, 
	.release = goldage_driver_release,
	.read = goldage_driver_read,
	.write = goldage_driver_write,
	.unlocked_ioctl = goldage_unlocked_ioctl,
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

	/*11. Map physical addr to virtual addr. */
	scull.baseaddr = ioremap(LED_BASEADDR,LED_DATOFFSET + 1);
	scull.conaddr = scull.baseaddr + LED_CONOFFSET;
	scull.dataddr = scull.baseaddr + LED_DATOFFSET;

	writel((readl(scull.conaddr) & ~(0x0f << 28)) | (0x1 << 28) , scull.conaddr);

	printk("devno:%d,major:%d,minor:%d\n",devno,MAJOR(devno),MINOR(devno));
	printk("goldage_driver_init.\n");
	return 0;

err0:
	unregister_chrdev_region(devno,count);
	return 0;
}

static void goldage_driver_exit(void)
{
	/* Release virtual addr.*/
	iounmap(scull.baseaddr);
	cdev_del(&scull.cdev);
	unregister_chrdev_region(devno,count);
	printk("Bye , kernel.\n");
}

module_init(goldage_driver_init);
module_exit(goldage_driver_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");
