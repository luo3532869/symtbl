/*
	File:	listsymtbl.h
	Author: Robin
	Time: 2015/03/12 10:17
*/
#ifndef __LISTSYMTBL_H__
#define __LISTSYMTBL_H__

#include "rolist.h"
#include "kvnode.h"

typedef int (*put_fn)(void *symtbl, ...);
typedef kv_t *(*get_fn)(void *symtbl, ...);
typedef void (*symfree_fn)(void *symtbl);
typedef int (*size_fn)(void *symtbl);
typedef int (*del_fn)(void *symtbl, ...);

typedef struct _symnode_t
{
	kv_t *key;
	kv_t *value;
	listnode lnode;
}symnode_t;

typedef struct _list_symtbl_t
{
	listhead symhead;
	put_fn put;
	get_fn get;
	size_fn size;
	del_fn del;
	symfree_fn free;
	int bforce;		/*if force the key or value has uniformd type*/
	type_kv tkey;
	int nsize;
}list_symtbl_t;

void *new_symtbl(int forcetype, type_kv tkey);

#endif
