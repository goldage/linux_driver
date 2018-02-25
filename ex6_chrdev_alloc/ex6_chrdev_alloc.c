#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

/* 1. Define base minor */
static unsigned minor = 0;
static int count = 1;
const char * name = "test_dev";

static dev_t devno;

static int goldage_driver_init(void){
	/* 2. Dynamically acquire and register the device number */
	if(alloc_chrdev_region(&devno,minor,count,name)){
		printk("alloc_chrdev_region failed.\n");
		return -1;
	}
	printk("goldage_driver_init.\n");
	printk("devno : %d\tmajor : %d\tminor : %d\t\n",devno,MAJOR(devno),MINOR(devno));
	return 0;
}

static void goldage_driver_exit(void){
	/*3. Release resource */
	unregister_chrdev_region(devno,count);
	printk("Bye , kernel.\n");
}

module_init(goldage_driver_init);
module_exit(goldage_driver_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");
