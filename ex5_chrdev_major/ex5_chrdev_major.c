#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

/* 1. Define dev number variable */
static unsigned major = 500;
static unsigned minor = 0;
static unsigned count = 1;
const char * name = "test_dev";

static dev_t devno;

static int goldage_driver_init(void){
	/* 2. Use a static way to acquire device number */	
	devno = MKDEV(major,minor);
	/* 3. Register device number */
	if(register_chrdev_region(devno , count , name)){
		printk("register_chrdev_region failed.\n");
		return -1;
	}

	printk("goldage_driver_init.\n");
	printk("devno : %d\tmajor : %d\tminor : %d\n",devno,MAJOR(devno),MINOR(devno));
	return 0;
}

static void goldage_driver_exit(void){
	/* 4. Release resource */
	unregister_chrdev_region(devno,count);
	printk("Bye , kernel.\n");
}

module_init(goldage_driver_init);
module_exit(goldage_driver_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");
