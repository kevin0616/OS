#include<linux/init.h>
#include<linux/module.h>
#include<linux/jiffies.h>
MODULE_LICENSE("Dual BSD/GPL");

unsigned long time_1, time_2;
ktime_t time_k1, time_k2;

static int hello_init(void){
	printk(KERN_INFO "Hello World\n");
	/* 1/HZ represents tick time in seconds, therefore *1000 */
	printk("tick time is %lu millisecond(s)", (unsigned long)1000/HZ);
	time_1=jiffies;
	time_k1=ktime_get_boottime();
	return 0;
}

static void hello_exit(void){
	time_2=jiffies;
	time_k2=ktime_get_boottime();
	printk("using jiffies: %lu millisecond(s)", 1000*(time_2-time_1)/HZ);
	/* value obtain from ktime_get_boottime is in nanoseconds, therefore /10^6 */
	printk("using values from timer: %lld millisecond(s)", (time_k2-time_k1)/1000000);
	printk(KERN_INFO "Goodbye, Cruel World\n");
}

module_init(hello_init);
module_exit(hello_exit);


