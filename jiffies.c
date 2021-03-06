/**
 * Jiffies.c
 *
 * Kernel module that communicates with /proc file system.
 * 
 * The makefile must be modified to compile this program.
 * Change the line "Jiffies.o"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/hash.h>
#include <linux/gcd.h>
#include <asm/param.h>
#include <linux/jiffies.h>


#define BUFFER_SIZE 128

#define PROC_NAME "jiffies"
#define MESSAGE "Jiffies Works\n"

/**
 * Function prototypes
 */
ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
        .owner = THIS_MODULE,
        .read = proc_read,
};


/* This function is called when the module is loaded. */
int proc_init(void)
{

        // creates the /proc/jiffies entry
        // the following function call is a wrapper for
        // proc_create_data() passing NULL as the last argument
        proc_create(PROC_NAME, 0, NULL, &proc_ops);

        printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
       //printk(KERN_INFO "proc_init jiffies %lu and Hz is %d\n", jiffies, HZ);


	return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void) {

        // removes the /proc/jiffies entry
        remove_proc_entry(PROC_NAME, NULL);
     
        //printk(KERN_INFO "proc_exit Jiffies is %lu\n", jiffies);
        printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
       
       
}

/**
 * This function is called each time the /proc/jiffies is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.
 *
 * params:
 *
 * file:
 * buf: buffer in user space
 * count:
 * pos:
 */
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        int rv = 0;
        char buffer[BUFFER_SIZE];
        static int completed = 0;

        if (completed) {
                completed = 0;
                return 0;
        }

        completed = 1;

        rv = sprintf(buffer, "Current value of jiffies is %lu\n", jiffies);
         //printk(KERN_INFO "Current value of jiffies %lu\n", jiffies);

        // copies the contents of buffer to userspace usr_buf
        copy_to_user(usr_buf, buffer, rv);

        return rv;
}


/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Jiffies Module");
MODULE_AUTHOR("SGG");



//============================================================
// Jiffies Make file
//============================================================
/*
obj-m += jiffies.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
    */
//============================================================
// 
//============================================================



//============================================================
// Jiffies Output
//============================================================
/*
osc@ubuntu:~/final-src-osc10e/ch2$ sudo insmod jiffies.ko
osc@ubuntu:~/final-src-osc10e/ch2$ cat /proc/jiffies
Jiffies in /proc/read is: 4297190631
osc@ubuntu:~/final-src-osc10e/ch2$ 
*/
//============================================================
// Jiffies End Output
//============================================================



//============================================================
// Jiffies Shell
//============================================================
/*
#!/bin/bash
​
# clear the message queue, then
# insert simple.ko into the kernel and remove it again, then
# display the messages as a result of doing so
​
sudo -p "osc"
sudo dmesg -c 
clear 
​
printf "beginning script...\n\n"
sudo insmod jiffies.ko

cat /proc/jiffies
sleep 8
cat /proc/jiffies
sleep 8
cat /proc/jiffies

sudo rmmod jiffies 
sudo dmesg 
printf "\n...end of script\n\n"
*/
//============================================================
// Jiffies Shell
//============================================================
