#include <linux/module.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>

/* 1. Define tasklet_struct . */
static struct tasklet_struct goldage_tasklet;

static const struct of_device_id goldage_dts_table[] = {
	{.compatible = "goldage,key"},
	{},
};

static irqreturn_t goldage_handler (int irqno , void * dev){
	printk("handler irqno:%d.\n",irqno);
	/* 3. Tasklet_schedule. */
	tasklet_schedule(&goldage_tasklet);
	return IRQ_HANDLED;
}

/* 4.Tasklet handler.*/
static void goldage_after_handler_tasklet(unsigned long args){
	printk("goldage_after_handle_tasklet.\n");
}

static int goldage_driver_probe(struct platform_device * pdev){
	struct resource * r = NULL;
	r = platform_get_resource(pdev,IORESOURCE_IRQ,0);

	if(request_irq(r->start,goldage_handler,IRQF_DISABLED | IRQF_TRIGGER_FALLING,"goldage-interrupt",NULL)){
		printk("request_irq failed.\n");
		free_irq(r->start,NULL);
		return -EFAULT;
	}

	/* 2. Initialize tasklet_struct. */
	tasklet_init(&goldage_tasklet,goldage_after_handler_tasklet,0);

	printk("match ok.\n");
	return 0;
}

static int goldage_driver_remove(struct platform_device * pdev){
	struct resource * r = NULL;
	r = platform_get_resource(pdev,IORESOURCE_IRQ,0);

	free_irq(r->start,NULL);
	printk("goldage_driver_remove.\n");
	return 0;
}

static struct platform_driver pdrv = {
	.probe = goldage_driver_probe,
	.remove = goldage_driver_remove,
	.driver = {
		.name = "goldage_device",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(goldage_dts_table),
	}
};

static int goldage_driver_init(void)
{
	platform_driver_register(&pdrv);
	printk("Hello world.\n");
	return 0;
}

static void goldage_driver_exit(void)
{
	platform_driver_unregister(&pdrv);
	printk("Bye , kernel.\n");
}

module_init(goldage_driver_init);
module_exit(goldage_driver_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("thegreaterlee@gmail.com");
