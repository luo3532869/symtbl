/*
	File:	binarysymtbl.c
	Author: Robin
	Time: 2015/03/23 09:35
*/

#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>
#include "rodebug.h"
#include "binarysymtbl.h"
#include "syscallpack.h"

static int bnyrank(void *symtbl, kv_t *key)
{
	binary_symtbl_t *st = (binary_symtbl_t *)symtbl;
	symnode_t *arr = st->array;
	kv_t *item;
	int ret;
	int l, u, m;
		
	l = 0;
	u = st->nelems - 1;

	while (l <= u)
	{
		m = l + (u - l) / 2;
		item = arr[m].key;

		ret = _OPS(item).cmp(item, key);
		if (0 == ret)
		{
			return m;
		}
		else if (ret < 0)
		{
			l = m + 1;
		}
		else
		{
			u = m -1;
		}
	}

	return l;
}

static symnode_t* _getnode(void *symtbl, kv_t *key, int *rank)
{
	binary_symtbl_t *st = (binary_symtbl_t *)symtbl;
	kv_t *ikey;
	int idx;
	symnode_t *sn;
	
	assert(NULL != key);

	if (st->nelems <= 0)
	{
		idx = 0;
		sn = NULL;
		goto OUT;
	}
	
	idx = st->rank(st, key);
	if (idx >= st->nelems)
	{
		sn = NULL;
		goto OUT;
	}	
	
	ikey = st->array[idx].key;
	
	if (0 == _OPS(ikey).cmp(ikey, key))
	{
		sn =  &(st->array[idx]);
	}
	else
	{
		sn = NULL;
	}
	
OUT:
	if (NULL != rank)
			*rank = idx;
	return sn;
}

static int _freesymnode(symnode_t *symnode)
{
	kv_t *kv;

	kv = symnode->key;
	freekvnode(kv);
	
	kv = symnode->value;
	freekvnode(kv);

	return 0;
}

static int _delnodefrarr(void *symtbl, int rank)
{
	binary_symtbl_t *st = (binary_symtbl_t *)symtbl;
	int i;

       if (rank < 0 || rank > st->nelems-1)
            return -1;
            
        _freesymnode(&(st->array[rank]));
        
	for (i = rank + 1; i < st->nelems; i++)
	{
		st->array[i-1] = st->array[i];
	}


	return 0;
}

static int _putinarray(void *symtbl, int rank)
{
	binary_symtbl_t *st = (binary_symtbl_t *)symtbl;
	int i;

	for (i = st->nelems - 1; i >= rank; i--)
	{
		st->array[i+1] = st->array[i];
	}

	return 0;
}

static kv_t *bnyget(void *symtbl, ...)
{
	binary_symtbl_t *st = (binary_symtbl_t *)symtbl;
	symnode_t *sn = NULL;

	va_list ap;
	va_start(ap, symtbl);
	kv_t *key;

	if (st->nelems <= 0)
	{
		return NULL;
	}
	
	key = NEWKVBYARG(ap);
	if (key->t != st->tkey)
	{
		ROHERE("Get imcompatible type of key");
		goto OUT;
	}
	
	sn = _getnode(st, key, NULL);

OUT:
	freekvnode(key);
	
	if (NULL == sn)
		return NULL;
	else
		return sn->value;


}

static int bnyput(void *symtbl, ...)
{
	symnode_t *symnode;
	binary_symtbl_t *st = (binary_symtbl_t *)symtbl;
	kv_t *key;
	kv_t *value;
	int rank;
	void *nalloc;
	va_list ap;

	va_start(ap, symtbl);
	
	key = NEWKVBYARG(ap);
	value = NEWKVBYARG(ap);

	if (key->t != st->tkey)
	{
		ROHERE("put imcompatible type of key.tkey:%d, tst:%d", key->t, st->tkey);
		goto OUT;
	}

	if (st->nelems >= st->nsize)
	{
		ROHERE("Binary array is full!");
		nalloc = REALLOC(st->array, 2 * st->nsize * sizeof(symnode_t));
		//FREE(st->array);
		st->array = (symnode_t *)nalloc;
		st->nsize = 2 * st->nsize;
	}
	
	if (NULL != (symnode = _getnode(st, key, &rank)))
	{
		freekvnode(symnode->value);
		freekvnode(key);
		symnode->value = value;
		return 0;
	}

	_putinarray(st, rank);
	symnode = &(st->array[rank]);
	symnode->key = key;
	symnode->value = value;
	st->nelems++;

       return 0;
       
OUT:
	freekvnode(key);
	freekvnode(value);
	return -1;
}



/*free the whole list*/
static void free_symtbl(void *symtbl)
{
	binary_symtbl_t *st = (binary_symtbl_t *)symtbl;
	int i;
	
	for (i = 0; i < st->nelems; i++)
	{
		_freesymnode(&(st->array[i]));
	}

	FREE(st->array);
}

/*delete one node from list*/
static int bnydel(void *symtbl, ...)
{
	binary_symtbl_t *st = (binary_symtbl_t *)symtbl;
	va_list ap;
	va_start(ap, symtbl);
	kv_t *key;
	symnode_t *sn;
	int rank;
	
	key = NEWKVBYARG(ap);
	if (key->t != st->tkey)
	{
	    ROHERE("Del imcompatible type of key.");
	    goto OUT;
	}
	
	sn = _getnode(st, key, &rank);
		
	if (NULL == sn)
	{
		return 0;
	}
	else
	{
		_delnodefrarr(st, rank);
		st->nelems--;
	}

OUT:
       freekvnode(key);
	return 0;
}

static int bnysize(void *symtbl)
{
	binary_symtbl_t *st = (binary_symtbl_t *)symtbl;

	return st->nelems;
}

void *new_symtbl(int forcetype, type_kv tkey)
{
	binary_symtbl_t *st;
	
	st = (binary_symtbl_t *)CALLOC(1, sizeof(binary_symtbl_t));

	st->array = (symnode_t *)CALLOC(INITIALSIZE, sizeof(symnode_t));
	
	st->put = bnyput;
	st->get = bnyget;
	st->rank = bnyrank;
	st->free = free_symtbl;
	st->del = bnydel;
	st->size = bnysize;

	st->nsize = INITIALSIZE;
	st->nelems = 0;

	st->bforce= 1;
	st->tkey = tkey;

	return st;
}
