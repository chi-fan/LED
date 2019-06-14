#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
 


 //定义资源
static struct resource LED_resource[] =
{
	[0] ={
		.start = 0x20AC000,
		.end =  0x20AC000 + 0x4 - 1,
		.flags = IORESOURCE_MEM,
	},
 
	[1] ={
		.start = 0x20AC004,
		.end =  0x20AC004 + 0x4 - 1,
		.flags = IORESOURCE_MEM,
	}

};
 
static void LED_release(struct device *dev)
{
	printk("LED_release\n");
	return ;
}
 
 
 //绑定资源
static struct platform_device LED_device=
{
    .name = "LED",
    .id = -1,
    .dev.release = LED_release,
    .num_resources = ARRAY_SIZE(LED_resource),
    .resource = LED_resource,
};
 
static int LED_init(void)
{
	printk("LED_init_device");
	return platform_device_register(&LED_device);
}
 
static void LED_exit(void)
{
	printk("LED_exit_device");
	platform_device_unregister(&LED_device);
	return;
}
 
MODULE_LICENSE("GPL");
module_init(LED_init);
module_exit(LED_exit);






