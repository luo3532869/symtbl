/*
    file:               	rolist.h
    description:	  	the header file of LIST 
    time:             	2014/12/04    11:26
    author:          	luopeng
*/
#ifndef __ROBIN_LIST_H__
#define __ROBIN_LIST_H__
#include <stdio.h>

typedef struct _node
{
    struct _node *prev;
    struct _node *next;
}listnode;

typedef listnode listhead;

#define GETADDR(name, member) ((struct name *)((char *)member - (int)(&(((struct name *)0)->lnode))))

#define  ROLIST_ITERATOR(head, p)    \
		for ((p) = ((head)->next); ((p) != (head)) && (NULL != (p)); (p) = (p)->next)

typedef int (*rocmp)(listnode *n1, listnode *n2);
typedef int (*rofree)(listnode *node);
typedef int (*romatch)(listnode *node, void *arg);

int rolist_addtail(listhead *h, listnode *node);
int rolist_addhead(listhead *h, listnode *node);
int rolist_del(listnode *node);
int rolist_inithead(listhead *h);
int rolist_isempty(listhead *h);
listnode *rolist_gethead(listhead *h);
int rolist_free(listhead *h, rofree nodefree);

#endif