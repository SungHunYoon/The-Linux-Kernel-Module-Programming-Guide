#include <linux/atomic.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/wait.h>

#include <asm/current.h>
#include <asm/errno.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define MESSAGE_LENGTH 80
static char message[MESSAGE_LENGTH];

static struct proc_dir_entry *our_proc_file;
#define PROC_ENTRY_FILENAME "sleep"

static ssize_t module_output(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
	static int finished = 0;
	int i;
	char output_msg[MESSAGE_LENGTH + 30];

	if (finished) {
		finished = 0;
		return 0;
	}

	sprintf(output_msg, "Last input:%s\n", message);
	for (i = 0; i < len && output_msg[i]; i++)
		put_user(output_msg[i], buf + i);

	finished = 1;
	return i;
}

static ssize_t module_input(struct file *file, const char __user *buf, size_t length, loff_t *offset)
{
	int i;

	for (i = 0; i < MESSAGE_LENGTH - 1 && i < length; i++)
		get_user(message[i], buf + i);
	message[i] = '\0';
	return i;
}

static atomic_t already_open = ATOMIC_INIT(0);

static DECLARE_WAIT_QUEUE_HEAD(waitq);

static int module_open(struct inode *inode, struct file *file)
{
	if (!atomic_cmpxchg(&already_open, 0, 1)) {
		try_module_get(THIS_MODULE);
		return 0;
	}

	if (file->f_flags & O_NONBLOCK)
		return -EAGAIN;

	try_module_get(THIS_MODULE);

	while (atomic_cmpxchg(&already_open, 0, 1)) {
		int i, is_sig = 0;

		wait_event_interruptible(waitq, !atomic_read(&already_open));

		for (i = 0; i < _NSIG_WORDS && !is_sig; i++)
			is_sig = current->pending.signal.sig[i] & ~current->blocked.sig[i];

		if (is_sig) {
			module_put(THIS_MODULE);
			return -EINTR;
		}
	}
	
	return 0;
}

static int module_close(struct inode *inode, struct file *file)
{
	atomic_set(&already_open, 0);

	wake_up(&waitq);

	module_put(THIS_MODULE);

	return 0;
}

#ifdef HAVE_PROC_OPS
static const struct proc_ops file_ops_4_our_proc_file = {
	.proc_read = module_output,
	.proc_write = module_input,
	.proc_open = module_open,
	.proc_release = module_close,
	.proc_lseek = noop_llseek,
};
#else
static const struct file_operations file_ops_4_our_proc_file = {
	.read = module_output,
	.write = module_input,
	.open = module_open,
	.release = module_close,
	.llseek = noop_llseek,
};
#endif

static int __init sleep_init(void)
{
	our_proc_file = proc_create(PROC_ENTRY_FILENAME, 0644, NULL, &file_ops_4_our_proc_file);
	if (our_proc_file == NULL) {
		pr_debug("Error: Could not initialize /proc/%s\n", PROC_ENTRY_FILENAME);
		return -ENOMEM;
	}
	proc_set_size(our_proc_file, 80);
	proc_set_user(our_proc_file, GLOBAL_ROOT_UID, GLOBAL_ROOT_GID);

	pr_info("/proc/%s created\n", PROC_ENTRY_FILENAME);

	return 0;
}

static void __exit sleep_exit(void)
{
	remove_proc_entry(PROC_ENTRY_FILENAME, NULL);
	pr_debug("/proc/%s removed\n", PROC_ENTRY_FILENAME);
}

module_init(sleep_init);
module_exit(sleep_exit);

MODULE_LICENSE("GPL");