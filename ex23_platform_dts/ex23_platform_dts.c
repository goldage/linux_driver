#include <linux/module.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/platform_device.h>

/* 3. Define dts table. */
static const struct of_device_id goldage_dts_table[ ] = {
	{.compatible = "goldage,led"},
	{},
};

static int goldage_driver_probe(struct platform_device* pdev){
	/* 4. Define resource structure.*/
	struct resource * r = NULL;
	r = platform_get_resource(pdev,IORESOURCE_MEM,0);

	printk("resource->start:%#x,resource->end:%#x.\n",r->start,r->end);
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
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(goldage_dts_table),
	},
};

static int goldage_driver_init(void){
	/* 1. Register platform_driver to kernel. */
	platform_driver_register(&pdrv);
	printk("Hello world.\n");
	return 0;
}

static void goldage_driver_exit(void){
	printk("Bye , kernel.\n");
	return;
}

module_init(goldage_driver_init);
module_exit(goldage_driver_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");
