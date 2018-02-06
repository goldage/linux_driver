#include <linux/module.h>
#include <linux/init.h>

static int goldage_driver_init(void){
	printk("Hello world.\n");
	return 0;
}

static void goldage_driver_exit(void){
	printk("Bye,kernel.\n");
	return 0;		
}

/* 1. Embellish entrance */
module_init(goldage_driver_init);
/* 2. Embellish exit */
module_exit(goldage_driver_exit);
/* 3. License */
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");
