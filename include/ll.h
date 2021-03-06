#ifndef K_LL_H
#define K_LL_H
#include <types.h>
struct llistnode {
	struct llistnode *next, *prev;
	void *entry;
};

struct llist {
	struct llistnode *head;
	mutex_t lock;
	char flags;
};

#define LL_ACTIVE   1
#define LL_ALLOC    2
#define LL_LOCKLESS 4
#define ll_is_active(list) ((list)->flags & LL_ACTIVE)

#define ll_entry(type,node) ((type)((node)->entry))

#define ll_for_each(list,curnode) \
		for(curnode=0; (curnode != 0 ? (curnode != (list)->head) : (addr_t)(curnode=(list)->head)); curnode=curnode->next)

#define ll_for_each_entry(list,curnode,type,_entry) \
		for(curnode=0; ((curnode != 0 ? (curnode != (list)->head) : (addr_t)(curnode=(list)->head)) && (_entry=ll_entry(type, curnode))); curnode=curnode->next)

#define ll_for_each_safe(list,curnode,_next) \
		for(curnode=0; ((curnode != 0 ? (curnode != (list)->head) : (addr_t)(curnode=(list)->head)) && (addr_t)(_next=curnode->next)); curnode=_next)

#define ll_for_each_entry_safe(list,curnode,_next,type,_entry) \
		for(curnode=0; ((curnode != 0 ? (curnode != (list)->head) : (addr_t)(curnode=(list)->head)) && (_entry=ll_entry(type, curnode)) && (addr_t)(_next=curnode->next)); curnode=_next)

#define ll_maybe_reset_loop(list,cur,next) \
		if((list)->head == cur) next=0

struct llist *ll_create(struct llist *list);
void ll_destroy(struct llist *list);
void ll_remove(struct llist *list, struct llistnode *node);
struct llistnode *ll_insert(struct llist *list, void *entry);
struct llist *ll_create_lockless(struct llist *list);

#endif
