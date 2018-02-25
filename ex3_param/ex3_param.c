#include <linux/module.h>
#include <linux/init.h>

static unsigned int goldage_int = 0;
static int goldage_arry[4] = {1,2,3,4};
static int goldage_num = 4;
static char* goldage_charp = "goldage_charp";

static int goldage_driver_init(void){
	int i = 0;
	
	printk("goldage_int : %d\n",goldage_int);

	for(i = 0; i < goldage_num ; i++){
		printk("goldage_arry[%d] : %d\n",i,goldage_arry[i]);
	}

	printk("goldage_charp : %s\n",goldage_charp);

	return 0;
}

static void goldage_driver_exit(void){
	printk("Bye , kernel.\n");
}

module_init(goldage_driver_init);
module_exit(goldage_driver_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");

module_param(goldage_int,int,0400);
MODULE_PARM_DESC(goldage_int,"Type : Integer.");

module_param(goldage_charp,charp,0000);
MODULE_PARM_DESC(goldage_charp,"Type : char*.");

module_param_array(goldage_arry,int,&goldage_num,0444);
MODULE_PARM_DESC(goldage_arry,"Num of array : 4.");
