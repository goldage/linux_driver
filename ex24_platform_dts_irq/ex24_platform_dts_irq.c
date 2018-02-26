#include <linux/module.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>

/* 3. Define dts match table.*/
static const struct of_device_id goldage_dts_table[] = {
	{.compatible = "goldage,led"},
	{},
};

static irqreturn_t goldage_handler(int irqno,void * dev){
	printk("handler irqno:%d.\n",irqno);
	return IRQ_HANDLED;
}
static int goldage_driver_probe(struct platform_device * pdev){
	/* 4. Get resource. */
	struct resource * r = NULL;
	r = platform_get_resource(pdev,IORESOURCE_IRQ,0);

	/* 5. Request irq. */
	if(request_irq(r->start,goldage_handler,IRQF_DISABLED | IRQF_TRIGGER_FALLING,"goldage_interrupt",NULL)){
		printk("request_irq failed.\n");
		free_irq(r->start,NULL);
		return -EFAULT;
	}

	printk("match ok.\n");
	return 0;
}

static int goldage_driver_remove(struct platform_device * pdev){
	struct resource * r = NULL;
	r = platform_get_resource(pdev,IORESOURCE_IRQ,0);

	/* 6. Free irq. */
	free_irq(r->start,NULL);
	printk("goldage_driver_remove.\n");
	return 0;
}

/* 2. Fill platform_driver structure.*/
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
	platform_driver_unregister(&pdrv);
	printk("Bye , kernel.\n");
}

module_init(goldage_driver_init);
module_exit(goldage_driver_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");
