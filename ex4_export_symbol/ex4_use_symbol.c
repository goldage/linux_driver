#include <linux/module.h>
#include <linux/init.h>
#include "symbol_export.h"

static int goldage_driver_init(void){
	printk("add(1,1):%d\n",test_add(1,1));
	return 0;
}

static void goldage_driver_exit(void){
	printk("Bye , kernel.\n");		
}

module_init(goldage_driver_init);
module_exit(goldage_driver_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");
