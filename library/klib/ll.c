/* ll.c - support functions for linked lists
 * These linked lists are stored as a circular doubly linked list, 
 * allowing us O(1) insert and remove.
 * 
 * WARNING - using the ll_for_each macros require a bit of extra work!
 * After the loop exits, curnode, entry and next are still set to
 * a value! Expecting them to be null will result in bugs. Also, 
 * if using the 'safe' macros, and you remove a node from the list, you
 * need to call ll_maybe_reset_loop before it reloops! */
#include <kernel.h>
#include <ll.h>
#include <mutex.h>
#include <task.h>

struct llistnode *ll_insert(struct llist *list, void *entry)
{
	assert(list && ll_is_active(list));
	struct llistnode *new = (struct llistnode *)kmalloc(sizeof(struct llistnode));
	if(!(list->flags & LL_LOCKLESS)) mutex_on(&list->lock);
	struct llistnode *old = list->head;
	list->head = new;
	list->head->next = old ? old : list->head;
	list->head->prev = old ? old->prev : list->head;
	if(old) {
		old->prev->next = list->head;
		old->prev = list->head;
	}
	list->head->entry = entry;
	/* In the unlikely event that list->head gets changed before the
	 * return statement, we don't want to return an incorrect pointer.
	 * Thus, we backup the new pointer and return it. */
	old = list->head;
	if(!(list->flags & LL_LOCKLESS)) mutex_off(&list->lock);
	return old;
}

void ll_remove(struct llist *list, struct llistnode *node)
{
	assert(list && node && ll_is_active(list));
	if(!(list->flags & LL_LOCKLESS)) mutex_on(&list->lock);
	if(list->head == node) {
		/* Lets put the head at the next node, in case theres a search. */
		list->head = node->next;
		/* Now, is this the only node in the list? */
		if(list->head == node) {
			list->head = 0;
			if(!(list->flags & LL_LOCKLESS)) mutex_off(&list->lock);
			kfree(node);
			return;
		}
	}
	node->prev->next = node->next;
	node->next->prev = node->prev;
	if(!(list->flags & LL_LOCKLESS)) mutex_off(&list->lock);
	kfree(node);
}

/* should list be null, we allocate one for us and return it. */
struct llist *ll_create(struct llist *list)
{
	if(list == 0) {
		list = (struct llist *)kmalloc(sizeof(struct llist));
		list->flags |= LL_ALLOC;
	} else
		list->flags = 0;
	create_mutex(&list->lock);
	list->head = 0;
	list->flags |= LL_ACTIVE;
	return list;
}

/* should list be null, we allocate one for us and return it. */
struct llist *ll_create_lockless(struct llist *list)
{
	list = ll_create(list);
	list->flags |= LL_LOCKLESS;
	return list;
}

void ll_destroy(struct llist *list)
{
	assert(list && !list->head);
	if(!ll_is_active(list))
		return;
	destroy_mutex(&list->lock);
	list->flags &= ~LL_ACTIVE;
	if(list->flags & LL_ALLOC)
		kfree(list);
}
