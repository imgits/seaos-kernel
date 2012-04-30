#include <kernel.h>
#include <memory.h>
#include <task.h>
#include <asm/system.h>
#include <dev.h>
#include <fs.h>
extern struct inode *devfs_root, *procfs_root;

int do_fs_stat(struct inode *i, struct fsstat *f)
{
	if(i && i->i_ops && i->i_ops->fsstat)
		return i->i_ops->fsstat(i, f);
	return -EINVAL;
}

int fs_stat(char *path, struct fsstat *f)
{
	struct inode *i = get_idir(path, 0);
	int ret = do_fs_stat(i, f);
	iput(i);
	return ret;
}

int rename(char *f, char *nname)
{
	if(!f || !nname) return -EINVAL;
	int ret = link(f, nname);
	if(ret >= 0)
		ret = unlink(f);
	return ret;
}

int sync_inode_tofs(struct inode *i)
{
	if(!i)
		return -EINVAL;
	int ret = -EINVAL;
	if(i && i->i_ops && i->i_ops->sync_inode)
		ret = i->i_ops->sync_inode(i);
	return ret;
}

struct inode *sys_create(char *path)
{
	return cget_idir(path, 0, current_task->cmask);
}

int create_node(struct inode *i, char *name, int mode, int maj, int min)
{
	if(!name)
		return -EINVAL;
	if(i->i_ops)
		if(i->i_ops->createnode)
			return i->i_ops->createnode(i, name, mode, maj, min);
	return 0;
}
