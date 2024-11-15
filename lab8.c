#include<linux/init.h>
#include<linux/module.h>
#include<linux/miscdevice.h>
#include<linux/sched.h>

MODULE_LICENSE("Dual BSD/GPL");

static int uid;



static int misc_open(struct inode* inodes, struct file* files){
	struct task_struct *p=current;
	uid=p->cred->uid.val;
	printk(KERN_INFO "%d called misc_open\n", uid);
	return 0;
}

static ssize_t misc_read(struct file *file, char __user *buf, size_t len, loff_t* offset){
	printk(KERN_INFO "%d called misc_read\n", uid);
	
	int tmp=uid;
	int l=0;
	char mybuff[20];
	snprintf(mybuff, sizeof(mybuff), "%d", tmp);
	if(tmp==0)l=1;
	else{
		while(tmp>0){
			l++;
			tmp/=10;
		}
	}
	mybuff[l]='\0';
	int left=l-*offset;

	if(left<=len){
		int c=copy_to_user(buf, &mybuff[*offset], left);
		if (c>0){
			return -1;
		}
		*offset+=left;
		return left;
	}else{
		int c=copy_to_user(buf, &mybuff[*offset], len);
		if (c>0){
			return -1;
		}
		*offset+=len;
		return len;
	}
}

static struct miscdevice my_dev;
static struct file_operations my_fops={
	.open=misc_open,
	.read=misc_read,
	.write=0,
	.release=0,
};

static int hello_init(void){
	int retval;
	
	my_dev.minor=MISC_DYNAMIC_MINOR;
	my_dev.name="lab8";
	my_dev.fops=&my_fops;
	my_dev.mode=0666;
	
	retval=misc_register(&my_dev);
	
	if(retval)return retval;
	printk(KERN_INFO "hello world\n");
	printk("lab8 got minor %i\n", my_dev.minor);
	
	return 0;
}

static void hello_exit(void){
	misc_deregister(&my_dev);
	printk(KERN_INFO "Goodbye, Cruel World\n");
}

module_init(hello_init);
module_exit(hello_exit);
