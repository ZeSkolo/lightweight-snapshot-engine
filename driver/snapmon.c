#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>
#include "../include/snapmon_ioctl.h"
static dev_t devno; static struct cdev cdev; static struct class *cls;
static DEFINE_MUTEX(lock); static unsigned long writes, snapshots, rollbacks;
static ssize_t snapmon_read(struct file *f,char __user *u,size_t n,loff_t *off){char b[160];int k;mutex_lock(&lock);k=scnprintf(b,sizeof(b),"writes=%lu snapshots=%lu rollbacks=%lu\n",writes,snapshots,rollbacks);mutex_unlock(&lock);return simple_read_from_buffer(u,n,off,b,k);}
static long snapmon_ioctl(struct file *f,unsigned int cmd,unsigned long arg){int e;if(cmd!=SNAPMON_IOC_EVENT)return -ENOTTY;if(copy_from_user(&e,(int __user *)arg,sizeof(e)))return -EFAULT;mutex_lock(&lock);if(e==SNAPMON_WRITE)writes++;else if(e==SNAPMON_SNAPSHOT)snapshots++;else if(e==SNAPMON_ROLLBACK)rollbacks++;else{mutex_unlock(&lock);return -EINVAL;}mutex_unlock(&lock);return 0;}
static const struct file_operations fops={.owner=THIS_MODULE,.read=snapmon_read,.unlocked_ioctl=snapmon_ioctl,.llseek=no_llseek};
static int __init snapmon_init(void){int r=alloc_chrdev_region(&devno,0,1,"snapmon");if(r)return r;cdev_init(&cdev,&fops);if((r=cdev_add(&cdev,devno,1)))goto unreg;cls=class_create("snapmon");if(IS_ERR(cls)){r=PTR_ERR(cls);goto del;}if(IS_ERR(device_create(cls,NULL,devno,NULL,"snapmon"))){r=-EINVAL;goto clsdel;}pr_info("snapmon loaded: major=%d minor=%d\n",MAJOR(devno),MINOR(devno));return 0;clsdel:class_destroy(cls);del:cdev_del(&cdev);unreg:unregister_chrdev_region(devno,1);return r;}
static void __exit snapmon_exit(void){device_destroy(cls,devno);class_destroy(cls);cdev_del(&cdev);unregister_chrdev_region(devno,1);pr_info("snapmon unloaded\n");}
module_init(snapmon_init);module_exit(snapmon_exit);MODULE_LICENSE("GPL");MODULE_AUTHOR("Valteri Vb");MODULE_DESCRIPTION("Snapshot engine event monitor character driver");
