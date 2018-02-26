#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>

static int goldage_driver_probe(struct platform_device* pdev){
	printk("match ok.\n");
	return 0;
}

static int goldage_driver_remove(struct platform_device * pdev){
	printk("goldage_driver_remove.\n");
	return 0;
}

/* 2. Fill platform_driver structure. */
static struct platform_driver pdrv = {
	.probe = goldage_driver_probe,
	.remove = goldage_driver_remove,
	.driver = {
		.name = "goldage_device",
	},
};

static int goldage_driver_init(void){
	/* 1. Register platform_driver to kernel.*/
	platform_driver_register(&pdrv);
	printk("Hello world.\n");
	return 0;
}

static void goldage_driver_exit(void){
	platform_driver_unregister(&pdrv);
	printk("Bye , kernel.\n");
}

module_init(goldage_driver_init);
module_exit(goldage_driver_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");
