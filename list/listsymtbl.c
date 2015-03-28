/*
	File:	listsymtbl.c
	Author: Robin
	Time: 2015/03/12 10:18
*/

#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>
#include "rodebug.h"
#include "listsymtbl.h"
#include "syscallpack.h"

static symnode_t* lstgetnode(void *symtbl, kv_t *key)
{
	list_symtbl_t *st = (list_symtbl_t *)symtbl;
	listnode *p;
	symnode_t *sn;
	kv_t *ikey;

	assert(NULL != key);
	
	ROLIST_ITERATOR(&(st->symhead), p)  
	{
		sn = GETADDR(_symnode_t, p);
		ikey = sn->key;
		if (EQUALTYPE(key, ikey) && (0 == _OPS(ikey).cmp(key, ikey)))
		{
			return sn;
		}
	}

	return NULL;
}

static kv_t *lstget(void *symtbl, ...)
{
	list_symtbl_t *st = (list_symtbl_t *)symtbl;
	symnode_t *sn;

	va_list ap;
	va_start(ap, symtbl);
	kv_t *key;
	
	key = NEWKVBYARG(ap);
	
	sn = lstgetnode(st, key);
	if (NULL != (_OPS(key)).free)
			(_OPS(key)).free(key);
	FREE(key);
	
	if (NULL == sn)
		return NULL;
	else
		return sn->value;
}

static int lstput(void *symtbl, ...)
{
	symnode_t *symnode;
	list_symtbl_t *st = (list_symtbl_t *)symtbl;
	kv_t *key;
	kv_t *value;
	va_list ap;

	va_start(ap, symtbl);
	
	key = NEWKVBYARG(ap);
	value = NEWKVBYARG(ap);

	if (st->bforce)
	{
		if (key->t != st->tkey)
		{
			ROHERE("put imcompatible type of key in symbol table.");
			goto OUT;
		}
	}
	if (NULL != (symnode = lstgetnode(st, key)))
	{
		freekvnode(symnode->value);
		freekvnode(key);
		symnode->value = value;
		return 0;
	}
	symnode = (symnode_t *)CALLOC(1, sizeof(symnode_t));

	symnode->key = key;
	symnode->value = value;
	st->nsize++;
	
	return rolist_addtail(&(st->symhead), &(symnode->lnode));
	
OUT:
	freekvnode(key);
	freekvnode(value);
	return -1;
}

static int lstfreenode(listnode *snode)
{
	symnode_t *sn;
	kv_t *kv;
	
	sn =  GETADDR(_symnode_t, snode);

	kv = sn->key;
	if (NULL != (_OPS(kv)).free)
		(_OPS(kv)).free(kv);
	FREE(kv);
	
	kv = sn->value;
	if (NULL != (_OPS(kv)).free)
		(_OPS(kv)).free(kv);
	FREE(kv);

	FREE(sn);

	return 0;
}

/*free the whole list*/
static void lstfree(void *symtbl)
{
	list_symtbl_t *st = (list_symtbl_t *)symtbl;
	listhead *head;

	head = &(st->symhead);

	rolist_free(head, lstfreenode);

}

/*delete one node from list*/
static int lstdel(void *symtbl, ...)
{
	list_symtbl_t *st = (list_symtbl_t *)symtbl;
	va_list ap;
	va_start(ap, symtbl);
	kv_t *key;
	symnode_t *sn;
	
	key = NEWKVBYARG(ap);
	
	sn = lstgetnode(st, key);
	
	freekvnode(key);
	 
	if (NULL == sn)
	{
		return 0;
	}
	else
	{
		rolist_del(&(sn->lnode));
		freekvnode(sn->key);
		freekvnode(sn->value);
		FREE(sn);
		st->nsize--;
	}

	return 0;
}

static int lstsize(void *symtbl)
{
	list_symtbl_t *st = (list_symtbl_t *)symtbl;

	return st->nsize;
}

void *new_symtbl(int forcetype, type_kv tkey)
{
	list_symtbl_t *st;
	
	st = (list_symtbl_t *)CALLOC(1, sizeof(list_symtbl_t));

	st->put = lstput;
	st->get = lstget;
	st->free = lstfree;
	st->del = lstdel;
	st->size = lstsize;
	rolist_inithead(&(st->symhead));
	st->bforce = forcetype;
	st->tkey = tkey;
	
	return st;
}

