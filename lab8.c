#include<linux/init.h>
#include<linux/module.h>
#include<linux/miscdevice.h>
#include<linux/sched.h>

MODULE_LICENSE("Dual BSD/GPL");

/*static kuid_t uid;
static int misc_open(struct inode* inodes, struct file* files){
	uid=current_uid();
	return 0;
}

static ssize_t misc_read(struct file *file, char __user *buf, size_t len, loff_t* offset){
	
	return 7;
}*/
/*
static struct miscdevice my_dev;
static struct file_operations test;*/
/*static struct file_operations my_fops={
	.open=0,
	.read=0,
	.write=0,
	.release=0
};*/

static int hello_init(void){
	/*int retval;
	
	my_dev.minor=MISC_DYNAMIC_MINOR;
	my_dev.name="lab8";
	my_dev.fops=&test;
	
	retval=misc_register(&my_dev);
	
	if(retval)return retval;
	printk("my:got minor %i\n", my_dev.minor);*/
	printk(KERN_INFO "hello\n");
	return 0;
}

static void hello_exit(void){
	printk(KERN_INFO "Goodbye, Cruel World\n");
}

module_init(hello_init);
module_exit(hello_exit);


