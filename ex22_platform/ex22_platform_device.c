#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>

void goldage_device_release(struct device * dev){
	printk("goldage_device_release.\n");
}

/* 2. Fill platform_device structure. */
static struct platform_device pdev = {
	.name = "goldage_device",
	.dev.release = goldage_device_release,
};

static int goldage_driver_init(void){
	/* 1. Register platform_device to kernel. */
	platform_device_register(&pdev);
	printk("Hello platform device.\n");
	return 0;
}

static void goldage_driver_exit(void){
	platform_device_unregister(&pdev);
	printk("Bye , kernel.\n");
}

module_init(goldage_driver_init);
module_exit(goldage_driver_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");
