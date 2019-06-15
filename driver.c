#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <asm/io.h>
 
static int major = 250;
static int minor=0;
static dev_t devno;
static struct class *cls;
static struct device *test_device;
                 

static void * data;
static void * direction;

 
static void LED_init(void)
{
	//不启用中断
	//读取direction寄存器数据
	printk("direction : %x\n", readl(direction));
	//将GPIO5_05引脚设置为输出引脚
	writel (readl(direction) | 0x20, direction);

	printk("direction : %x\n", readl(direction));

	//读取data寄存器数据
	printk("data: %x\n", readl(data));
	//将GPIO5_05引脚电平拉低
	writel (readl(data) & 0xFDF, data);
	printk("data: %x\n", readl(data));
}
 
void LED_on(void)
{
	;
}
 
void LED_off(void)
{
	;
}
 
static void LED_unmap(void)
{
	iounmap(direction);
	iounmap(data);
}
 
static int LED_open (struct inode *inode, struct file *filep)
{
	LED_on();
	return 0;
}
 
static int LED_release(struct inode *inode, struct file *filep)
{
	 LED_off();
	 return 0;
}
 
 
 //挂载函数
static struct file_operations LED_ops=
{
	.open     = LED_open,
	.release  = LED_release,
};
 


 //将设备与驱动相互绑定
static int LED_probe(struct platform_device *pdev)
{
	int ret;	
	printk("LED match ok!\n");
	

	//映射地址
	data   			= ioremap(pdev->resource[0].start, pdev->resource[0].end - pdev->resource[0].start);
	direction    	= ioremap(pdev->resource[1].start, pdev->resource[1].end - pdev->resource[1].start);

	//注册设备号
	devno = MKDEV(major,minor);
	ret = register_chrdev(major,"LED",&LED_ops);
 
	cls = class_create(THIS_MODULE, "myclass");
	if(IS_ERR(cls))
	{
		unregister_chrdev(major,"LED");
		return -EBUSY;
	}
 
	//注册设备节点
	test_device = device_create(cls,NULL,devno,NULL,"LED");//mknod /dev/hello
	if(IS_ERR(test_device))
	{
		class_destroy(cls);
		unregister_chrdev(major,"LED");
		return -EBUSY;
	}
	
	LED_init();	
	return 0;
}
 
static int LED_remove(struct platform_device *pdev)
{
	LED_unmap();
	device_destroy(cls,devno);
	class_destroy(cls);	
	unregister_chrdev(major,"LED");
 
	return 0;
}
 
 
static struct platform_driver LED_driver=
{
    .driver.name = "LED",
    .probe = LED_probe,
    .remove = LED_remove,
};
 
 
static int __init LED_init_2(void)
{
	printk("LED_init_dirve");
	
	//注册驱动
	return platform_driver_register(&LED_driver);
}
 
static void __exit LED_exit_2(void)
{
	printk("LED_exit_drive");

	//卸载驱动
	platform_driver_unregister(&LED_driver);
	
	return;
}
 
 
MODULE_LICENSE("GPL");
module_init(LED_init_2);
module_exit(LED_exit_2);
