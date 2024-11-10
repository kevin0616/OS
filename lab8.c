#include<linux/init.h>
#include<linux/module.h>
#include<linux/miscdevice.h>
#include<linux/sched.h>

MODULE_LICENSE("Dual BSD/GPL");

static kuid_t uid;
char *mybuff;
int l;

static int misc_open(struct inode* inodes, struct file* files){
	struct task_struct *p=current;
	uid=p->cred->uid;
	printk(KERN_INFO "%d\n", uid.val);
	int tmp=127383;
	
	char buf[10];
	if(tmp==0)buf[0]='0';
	int cnt=0;
	while(tmp>0){
		mybuff[l++]=tmp%10;
		tmp/=10;
	}
	mybuff[l]='\0';
	printk(KERN_INFO "%s\n", buf);

	printk(KERN_INFO "myopen\n");
	return 0;
}

static ssize_t misc_read(struct file *file, char __user *buf, size_t len, loff_t* offset){
	printk(KERN_INFO "myread\n");
	//printk(KERN_INFO "%d %d\n",(int)len, (int)*offset);
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
	.release=0
};

static int hello_init(void){
	int retval;
	
	my_dev.minor=MISC_DYNAMIC_MINOR;
	my_dev.name="lab8";
	my_dev.fops=&my_fops;
	
	retval=misc_register(&my_dev);
	
	if(retval)return retval;
	printk("my:got minor %i\n", my_dev.minor);
	printk(KERN_INFO "hello\n");
	return 0;
}

static void hello_exit(void){
	misc_deregister(&my_dev);
	printk(KERN_INFO "Goodbye, Cruel World\n");
}

module_init(hello_init);
module_exit(hello_exit);

