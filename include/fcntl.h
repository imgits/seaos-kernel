#ifndef _FCNTL_H
#define _FCNTL_H
#include <kernel.h>
#include <memory.h>
#include <task.h>
#include <fs.h>
#include <sys/fcntl.h>

struct flock *create_flock(int type, int whence, off_t start, size_t len);
int can_flock(struct inode *file, struct flock *l);
int disengage_flock(struct inode *file, struct flock *l);
int engage_flock(struct inode *file, struct flock *l, int);
int can_access_file(struct inode *file, int access);
int fcntl_getlk(struct file *f, int arg);
int fcntl_setlk(struct file *f, int arg);
int fcntl_setlkw(struct file *f, int arg);
void destroy_flocks(struct inode *f);

#endif
