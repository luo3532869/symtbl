/*
	File:	bstsymtbl.h
	Author: Robin
	Time: 2015/03/25 14:10
*/
#ifndef __BSTSYMTBL_H__
#define __BSTSYMTBL_H__

#include "kvnode.h"

#define INITIALSIZE	100

typedef int (*put_fn)(void *symtbl, ...);
typedef kv_t *(*get_fn)(void *symtbl, ...);
typedef void (*symfree_fn)(void *symtbl);
typedef int (*size_fn)(void *symtbl);
typedef int (*del_fn)(void *symtbl, ...);
typedef int (*rank_fn)(void *symtbl, kv_t *key);
typedef kv_t *(*min_fn)(void *symtbl);
typedef kv_t *(*floor_fn)(void *symtbl, kv_t *key);
typedef kv_t *(*select_fn)(void *symtbl, int k);

typedef struct _symnode_t
{
	kv_t *key;
	kv_t *value;
	struct _symnode_t *left;
	struct _symnode_t *right;
	int nelems;
}symnode_t;

typedef struct _bst_symtbl_t
{
	symnode_t *root;
	
	put_fn put;
	get_fn get;
	rank_fn rank;
	min_fn min;
	floor_fn floor;
	select_fn select;
	size_fn size;
	del_fn del;				/*delete one node from table*/
	symfree_fn free;			/*free the symbol table*/

	int bforce;		/*if force the key or value has uniformd type*/
	type_kv tkey;
}bst_symtbl_t;

void *new_symtbl();

#endif
