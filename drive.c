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
                 

static void * Control_Register;
static void * Status_Register;
static void * Interrupt_Register;
static void * Sample_Register;
static void * Period_Register;
static void * counter_Register;  


 
#define  MAGIC_NUMBER    'k'
#define  PWM_ON    _IO(MAGIC_NUMBER    ,0)
#define  PWM_OFF   _IO(MAGIC_NUMBER    ,1)
#define  PWM_FREQ   _IO(MAGIC_NUMBER   ,2)
#define  PWM_Duty_cycle _IO(MAGIC_NUMBER   ,3)
 
static void PWM_3_init(void)
{
	//不启用中断
	writel (0x0000 ,Interrupt_Register);

	writel (0x0005 ,Sample_Register);
	writel (0x001E ,Period_Register);
	writel (0x0309 ,Control_Register);
}
 
void PWM_3_on(void)
{

}
 
void PWM_3_off(void)
{

}
 
static void PWM_3_unmap(void)
{
	iounmap(Control_Register);
	iounmap(Status_Register);
	iounmap(Interrupt_Register);
	iounmap(Sample_Register);
	iounmap(Period_Register);
	iounmap(counter_Register);
}
 
static int PWM_3_open (struct inode *inode, struct file *filep)
{
	PWM_3_on();
	return 0;
}
 
static int PWM_3_release(struct inode *inode, struct file *filep)
{
	 PWM_3_off();
	 return 0;
}
 

static void PWM_3_freq(unsigned long arg)
{

 
}

static void PWM_3_Duty_cycle(unsigned long arg)
{

 
}
 
static long PWM_3_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
	switch(cmd)
	{
		case PWM_ON:
			PWM_on();
			break;
		case PWM_OFF:
			PWM_off();
			break;
		case PWM_FREQ:
			PWM_3_freq(arg);
			break;
		case PWM_Duty_cycle:
			PWM_3_Duty_cycle(arg);
			break;
		default :
			return -EINVAL;
	}
	return 0;
}
 
 //挂载函数
static struct file_operations PWM_ops=
{
	.open     = PWM_3_open,
	.release  = PWM_3_release,
	.unlocked_ioctl  = PWM_3_ioctl,
};
 


 //将设备与驱动相互绑定
static int PWM_3_probe(struct platform_device *pdev)
{
	int ret;	
	printk("PWM_3 match ok!");
	

	//映射地址
	Control_Register    = ioremap(pdev->resource[0].start, pdev->resource[0].end - pdev->resource[0].start);
	Status_Register     = ioremap(pdev->resource[1].start, pdev->resource[1].end - pdev->resource[1].start);
	Interrupt_Register  = ioremap(pdev->resource[2].start, pdev->resource[2].end - pdev->resource[2].start);
	Sample_Register     = ioremap(pdev->resource[3].start, pdev->resource[3].end - pdev->resource[3].start);
	Period_Register     = ioremap(pdev->resource[4].start, pdev->resource[4].end - pdev->resource[4].start);
	counter_Register    = ioremap(pdev->resource[5].start, pdev->resource[5].end - pdev->resource[5].start);

	//注册设备号
	devno = MKDEV(major,minor);
	ret = register_chrdev(major,"PWM_3",&PWM_ops);
 
	cls = class_create(THIS_MODULE, "myclass");
	if(IS_ERR(cls))
	{
		unregister_chrdev(major,"PWM_3");
		return -EBUSY;
	}
 
	//注册设备节点
	test_device = device_create(cls,NULL,devno,NULL,"PWM_3");//mknod /dev/hello
	if(IS_ERR(test_device))
	{
		class_destroy(cls);
		unregister_chrdev(major,"PWM_3");
		return -EBUSY;
	}
	
	PWM_3_init();	
	return 0;
}
 
static int PWM_3_remove(struct platform_device *pdev)
{
	PWM_unmap();
	device_destroy(cls,devno);
	class_destroy(cls);	
	unregister_chrdev(major,"PWM_3");
 
	return 0;
}
 
 
static struct platform_driver PWM_driver=
{
    .driver.name = "PWM_3",
    .probe = PWM_3_probe,
    .remove = PWM_3_remove,
};
 
 
static int __init PWM_init(void)
{
	printk("PWM_3_init_dirve");
	
	//注册驱动
	return platform_driver_register(&PWM_driver);
}
 
static void __exit PWM_exit(void)
{
	printk("PWM_3_exit_drive");

	//卸载驱动
	platform_driver_unregister(&PWM_driver);
	
	return;
}
 
 
MODULE_LICENSE("GPL");
module_init(PWM_init);
module_exit(PWM_exit);
