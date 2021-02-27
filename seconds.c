/**
 * seconds.c
 *
 * Kernel module that communicates with /proc file system.
 * 
 * The makefile must be modified to compile this program.
 * Change the line "seconds.o"
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

#define PROC_NAME "seconds"
#define MESSAGE "Seconds is working\n"

unsigned long full=0;

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

        // creates the /proc/seconds entry
        // the following function call is a wrapper for
        // proc_create_data() passing NULL as the last argument
        proc_create(PROC_NAME, 0, NULL, &proc_ops);
        printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

        full = (long)jiffies;
        //printk(KERN_INFO "proc_init jiffies %lu and Hz is %d\n", jiffies, HZ);


	return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void) {

        // removes the /proc/seconds entry
        remove_proc_entry(PROC_NAME, NULL);


        //printk(KERN_INFO "proc_exit Jiffies is %lu\n", jiffies);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
       
       
}

/**
 * This function is called each time the /proc/secodns is read.
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
        int seconds = 0;
        char buffer[BUFFER_SIZE];
        static int completed = 0;

        if (completed) {
                completed = 0;
                return 0;
        }

        completed = 1;

        seconds = (long)(jiffies-full)/HZ;
        rv = sprintf(buffer, "Elapsed Seconds: %lu\n", seconds);

         //printk(KERN_INFO "proc_read Jiffies is %lu\n", jiffies);

        // copies the contents of buffer to userspace usr_buf
        copy_to_user(usr_buf, buffer, rv);

        return rv;
}


/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Seconds Module");
MODULE_AUTHOR("SGG");

//============================================================
// Seconds Make file
//============================================================
/*
obj-m += seconds.o
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
osc@ubuntu:~/final-src-osc10e/ch2$ sudo insmod seconds.ko
osc@ubuntu:~/final-src-osc10e/ch2$ cat /proc/seconds
Elapsed Seconds: 4
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
sudo insmod seconds.ko

cat /proc/seconds
sleep 4
cat /proc/seconds
sleep 4
cat /proc/seconds

sudo rmmod seconds
sudo dmesg 
printf "\n...end of script\n\n"
*/
//============================================================
// Jiffies Shell
//============================================================
