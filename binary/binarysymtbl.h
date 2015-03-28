/*
	File:	binarysymtbl.h
	Author: Robin
	Time: 2015/03/23 09:34
*/
#ifndef __LISTSYMTBL_H__
#define __LISTSYMTBL_H__

#include "kvnode.h"

#define INITIALSIZE	100

typedef int (*put_fn)(void *symtbl, ...);
typedef kv_t *(*get_fn)(void *symtbl, ...);
typedef void (*symfree_fn)(void *symtbl);
typedef int (*size_fn)(void *symtbl);
typedef int (*del_fn)(void *symtbl, ...);
typedef int (*rank_fn)(void *symtbl, kv_t *key);

typedef struct _symnode_t
{
	kv_t *key;
	kv_t *value;
}symnode_t;

typedef struct _list_symtbl_t
{
	symnode_t *array;
	
	put_fn put;
	get_fn get;
	rank_fn rank;
	size_fn size;
	del_fn del;				/*delete one node from table*/
	symfree_fn free;			/*free the symbol table*/
	
	int nelems;
	int nsize;

	int bforce;		/*if force the key or value has uniformd type*/
	type_kv tkey;
}binary_symtbl_t;

void *new_symtbl();

#endif
