#include <kernel.h>
#include <syscall.h>
#include <isr.h>
#include <task.h>
#include <dev.h>
#include <fs.h>
#include <sys/stat.h>
#include <mod.h>
#include <sys/sysconf.h>

extern int current_hz;

long sys_sysconf(int cmd)
{
	int ret = -EINVAL;
	switch(cmd)
	{
		case _SC_PAGESIZE:
			ret = PAGE_SIZE;
			break;
		case _SC_CLK_TCK:
			ret = current_hz;
			break;
		case _SC_PHYS_PAGES:
			ret = pm_num_pages;
			break;
		case _SC_NGROUPS_MAX:
			ret = 1000;
			break;
		case _SC_CHILD_MAX:
			ret = 0;
			break;
		default:
			printk(1, "[sysconf]: %d gave unknown specifier: %d\n", current_task->pid, cmd);
	}
	return ret;
}

int sys_gethostname(char *buf, int len)
{
	if(!buf || !len)
		return -EINVAL;
	strncpy(buf, "seaos", len);
	return 0;
}

int sys_uname(struct utsname *name)
{
	if(!name)
		return -EINVAL;
	strcpy(name->sysname, "seaos");
	strcpy(name->nodename, "");
	strcpy(name->release, "0.2");
	strcpy(name->version, "eclipse");
	strcpy(name->machine, "i586-pc");
	strcpy(name->domainname, "");
	return 0;
}
