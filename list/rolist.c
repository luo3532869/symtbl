/*
    file:               rolist.c
    time:             2014/12/04    11:29
    author:          luopeng
*/
#include "rolist.h"

int rolist_inithead(listhead *h)
{
	if (NULL == h)	
		return -1;
	h->prev = h->next = h;
	return 0;
}

int rolist_isempty(listhead *h)
{
	if ((h->prev == h) && (h->next == h))
		return 1;
	else
		return 0;
}

int rolist_addtail(listhead *h, listnode *node)
{
	listnode *tail;
	
	if ((NULL == node) || (NULL == h))
		return -1;
	
	node->next = h;
	
	if (h == h->next)
	{
		h->next = node;
		h->prev = node;
		node->prev = h;
	}
	else
	{
		tail = h->prev;
		h->prev = node;
		tail->next = node;
		node->prev = tail;
	}
	
	return 0;
}

int rolist_addhead(listhead *h, listnode *node)
{
	listnode *h1;
	
	if ((NULL == node) || (NULL == h))
		return -1;
		
	node->prev = h;
	if (h == (h1 = h->next))
	{
		h->next = node;
		h->prev = node;
		node->next = h;
	}
	else
	{
		h->next = node;
		h1->prev = node;
		node->next = h1;
	}
	return 1;
}

int rolist_del(listnode *node)
{
	if (NULL == node)
		return -1;
		
	node->prev->next = node->next;
	if (NULL != node->next)
	{
		node->next->prev = node->prev;
	}

	return 0;
}

int rolist_free(listhead *h, rofree nodefree)
{
	listnode *p;
	listnode *n;
	for ((p) = ((h)->next); ((p) != (h)) && (NULL != (p)); )
	{
		rolist_del(p);
		n = p->next;
		nodefree(p);
		p = n;
	}

	return 0;
}

listnode *rolist_gethead(listhead *h)
{
	listnode *p;
	
	if (rolist_isempty(h))
		return NULL;
	p = h->next;
	rolist_del(p);

	return p;
}

listnode *_getsmallest(listhead *h, rocmp cmp)
{
	listnode *small;
	listnode *p;
	
	if (rolist_isempty(h))
		return NULL;
	
	small = h->next;
	ROLIST_ITERATOR(h, p)
	{
		if (cmp(small, p) > 0)
			small = p;
	}
	return small;
}

/*insert sort*/
int rolist_sort(listhead *h, rocmp cmp)
{
	listhead tp;   /*temp header*/
	listnode *small;
	
	rolist_inithead(&tp);
	
	while (!rolist_isempty(h))
	{
		small = _getsmallest(h, cmp);
		rolist_del(small);
		rolist_addtail(&tp, small);
	}
	
	h->next = tp.next;
	h->prev = tp.prev;
	tp.next->prev = h;
	tp.prev->next = h;
	
	return 0;
}

