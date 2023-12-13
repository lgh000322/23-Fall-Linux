#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/unistd.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

#define MAX_SIZE 1

static dev_t my_dev;
static struct cdev my_cdev;

static char* device_buf=NULL;

int device_open(struct inode *inode, struct file *filp);
int device_release(struct inode *inode,struct file *filp);
ssize_t device_read(struct file *filp,char* buf, size_t count,loff_t *f_pos);
ssize_t device_write(struct file *filp,const char *buf,size_t count,loff_t *f_pos);

static struct file_operations fops={
	.read= device_read,
	.write= device_write,
	.open= device_open,
	.release= device_release,
};

int device_open(struct inode *inode, struct file *filp){
	printk(KERN_INFO "My device is opened\n");
	return 0;
}

int device_release(struct inode *inode,struct file *filp){
	printk(KERN_INFO "My device is released\n");
	return 0;
}

ssize_t device_write(struct file *filp,const char *buf,size_t count,loff_t *f_pos){
	ssize_t result=copy_from_user(device_buf,buf,count);
	
	if(result<0){
		printk(KERN_ALERT "[%s] copy_from_user failed: %ld\n",__func__,result);
		return result;
	}
	device_buf[0]+=1;
	return count;
}

ssize_t device_read(struct file *filp, char *buf,size_t count,loff_t *fpos){
	ssize_t result=copy_to_user(buf,device_buf,count);
	if(result<0){
		printk(KERN_ALERT "[%s] copy_to_user failed:%ld\n",__func__,result);
		return result;
	}
	return count;
}

int device_init(void){
	if(alloc_chrdev_region(&my_dev,0,1,"my_virt_dev")){
		printk(KERN_ALERT "[%s] alloc_chrdev_region failed\n",__func__);
		return -1;
	}
	printk("My virtual dev's Major number=%d\n",MAJOR(my_dev));

	cdev_init(&my_cdev,&fops);

	if(cdev_add(&my_cdev,my_dev,1)){
		printk(KERN_ALERT "[%s] cdev_add failed\n",__func__);
		return -1;
	}

	device_buf=(char*)kmalloc(MAX_SIZE, GFP_KERNEL);
	if(device_buf!=NULL)
		memset(device_buf,0,MAX_SIZE);
	return 0;
}

void device_exit(void){
	cdev_del(&my_cdev);
	unregister_chrdev_region(my_dev,1);
	if(device_buf!=NULL)
		kfree(device_buf);
}

module_init(device_init);
module_exit(device_exit);
MODULE_LICENSE("GPL");
