#include <linux/module.h>
#include <linux/init.h>

int test_add(int a,int b){
	return a + b;
}
EXPORT_SYMBOL(test_add);

static int goldage_driver_init(void){
	printk("Hello world.\n");
	return 0;
}

static void goldage_driver_exit(void){
	printk("Bye , kernel.\n");
}

module_init(goldage_driver_init);
module_exit(goldage_driver_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");
