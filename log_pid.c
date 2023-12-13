#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/pid.h>

static int init_logpid(void){
	printk(KERN_ALERT "Hello.\n");
	return 0;
}

static void exit_logpid(void){
	printk(KERN_ALERT "BYE\n");
}

void log_pid(struct pid* p){
	printk("fork: pid_vnr=%d, pid_nr=%d\n",pid_vnr(p),pid_nr(p));
}

module_init(init_logpid);
module_exit(exit_logpid);
MODULE_LICENSE("GPL");
