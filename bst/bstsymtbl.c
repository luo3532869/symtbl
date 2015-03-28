/*
	File:	bstsymtbl.c
	Author: Robin
	Time: 2015/03/25 14:10
*/

#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>
#include "rodebug.h"
#include "bstsymtbl.h"
#include "syscallpack.h"

static int _treesize(symnode_t *t)
{
	if (NULL == t)
		return 0;
	else
		return t->nelems;
}

static int _rank(symnode_t *t, kv_t *key)
{
	int cmp;
	
	if (NULL == t)
		return 0;

	cmp = _OPS(key).cmp(key, t->key);
	if (cmp < 0)
		return _rank(t->left, key);
	else if(cmp > 0)
		return _treesize(t->left) + 1 + _rank(t->right, key);
	else
		return _treesize(t->left);
}

static symnode_t *_min(symnode_t *t)
{
	if (NULL == t)
		return NULL;

	if (NULL == t->left)
		return t;
	else
		return _min(t->left);
}

static symnode_t *_floor(symnode_t *t, kv_t *key)
{
	int cmp;
	symnode_t *s;
	
	if (NULL == t)
		return NULL;

	cmp = _OPS(key).cmp(key, t->key);

	if (cmp < 0)
		return _floor(t->left, key);
	else if (cmp > 0)
	{
		s = _floor(t->right, key);
		if (NULL == s)
			return t;
		else
			return s;
	}
	else
		return t;
}

static symnode_t *_select(symnode_t *t, int k)
{
	int nleft;
	symnode_t *s;
	
	if (NULL == t)
		return NULL;

	nleft = _treesize(t->left);

	if (k < nleft)
		return _select(t->left, k);
	else if (k > nleft)
		return _select(t->right, k - nleft - 1);
	else
		return t;
}

static int bstrank(void *symtbl, kv_t *key)
{
	bst_symtbl_t *st = (bst_symtbl_t *)symtbl;
	
	return _rank(st->root, key);	
}

static int _freesymnode(symnode_t *symnode)
{
	kv_t *kv;

	kv = symnode->key;
	freekvnode(kv);
	
	kv = symnode->value;
	freekvnode(kv);

	FREE(symnode);
	return 0;
}

static symnode_t *_get(symnode_t *t, kv_t *key)
{
	int cmp;
	
	if (NULL == t) 
		return NULL;

	cmp = _OPS(key).cmp(key, t->key);

	if (cmp < 0)
		return _get(t->left, key);
	else if (cmp > 0)
		return _get(t->right, key);
	else
		return t;
}

static kv_t *bstget(void *symtbl, ...)
{
	bst_symtbl_t *st = (bst_symtbl_t *)symtbl;
	symnode_t *sn = NULL;

	va_list ap;
	va_start(ap, symtbl);
	kv_t *key;
	
	key = NEWKVBYARG(ap);
	if (key->t != st->tkey)
	{
		ROHERE("Get imcompatible type of key");
		goto OUT;
	}
	
	sn = _get(st->root, key);

OUT:
	freekvnode(key);
	
	if (NULL == sn)
		return NULL;
	else
		return sn->value;


}

static symnode_t * _newsymnode(kv_t *key, kv_t *value, int n)
{
	symnode_t *newnode;

	newnode = (symnode_t *)CALLOC(1, sizeof(symnode_t));
	newnode->key = key;
	newnode->value = value;
	newnode->left = NULL;
	newnode->right = NULL;
	newnode->nelems = n;

	return newnode;
}

static symnode_t *_put(symnode_t *node, kv_t *key, kv_t *value)
{
	int cmp;
	
	if (NULL == node)
	{
		return _newsymnode(key, value, 1);
	}

	cmp = _OPS(key).cmp(key, node->key);

	if (cmp < 0) 
		node->left = _put(node->left, key, value);
	else if (cmp > 0) 
		node->right = _put(node->right, key, value);
	else
	{
		freekvnode(key);
		freekvnode(node->value);
		node->value = value;
	}

	node->nelems = _treesize(node->right) + _treesize(node->left) + 1;

	return node;
}

static int bstput(void *symtbl, ...)
{
	bst_symtbl_t *st = (bst_symtbl_t *)symtbl;
	kv_t *key;
	kv_t *value;
	va_list ap;

	va_start(ap, symtbl);
	
	key = NEWKVBYARG(ap);
	value = NEWKVBYARG(ap);

	if (key->t != st->tkey)
	{
		ROHERE("put imcompatible type of key.tkey:%d, tst:%d", key->t, st->tkey);
		freekvnode(key);
		freekvnode(value);
		return -1;
	}

	st->root = _put(st->root, key, value);

	return 0;
}


static void _freetree(symnode_t *t)
{
	if (NULL == t)
		return;

	_freetree(t->left);
	_freetree(t->right);

	_freesymnode(t);
}

/*free the whole list*/
static void free_symtbl(void *symtbl)
{
	bst_symtbl_t *st = (bst_symtbl_t *)symtbl;
	
	_freetree(st->root);
}

static symnode_t *_delmin(symnode_t *t)
{
	symnode_t *s;
	
	if (NULL == t)
		return NULL;

	if (NULL == t->left)
	{
		s = t->right;
		return s;
	}
	else
	{
		t->left = _delmin(t->left);
		t->nelems = _treesize(t->left) + _treesize(t->right) + 1;
		return t;
	}
	
}

static symnode_t *_del(symnode_t *t, kv_t *key)
{
	int cmp;
	symnode_t *s;
	
	if (NULL == t)
		return NULL;
		
	cmp = _OPS(key).cmp(key, t->key);

	if (cmp < 0)
		t->left = _del(t->left, key);
	else if (cmp > 0)
		t->right = _del(t->right, key);
	else
	{
		if (NULL == t->left)
		{	
			s = t->right;
			_freesymnode(t);
			return s;
		}
		if (NULL == t->right)
		{
			s = t->left;
			_freesymnode(t);
			return s;
		}

		s = t;
		t = _min(s->right);
		t->right = _delmin(s->right);
		t->left = s->left;
		_freesymnode(s);
	}

	t->nelems = _treesize(t->left) + _treesize(t->right) + 1;

	return t;
}

/*delete one node from list*/
static int bstdel(void *symtbl, ...)
{
	bst_symtbl_t *st = (bst_symtbl_t *)symtbl;
	va_list ap;
	va_start(ap, symtbl);
	kv_t *key;
	
	key = NEWKVBYARG(ap);
	if (key->t != st->tkey)
	{
	    ROHERE("Del imcompatible type of key.");
	    goto OUT;
	}
	
	st->root = _del(st->root, key);

OUT:
       freekvnode(key);
	return 0;
}



static int bstsize(void *symtbl)
{
	bst_symtbl_t *st = (bst_symtbl_t *)symtbl;

	return _treesize(st->root);
}


static kv_t *bstmin(void *symtbl)
{
	bst_symtbl_t *st = (bst_symtbl_t *)symtbl;
	symnode_t *m;

	m = _min(st->root);

	return m->value;
}

static kv_t *bstfloor(void *symtbl, kv_t *key)
{
	bst_symtbl_t *st = (bst_symtbl_t *)symtbl;
	symnode_t *m;

	m = _floor(st->root, key);

	return m->value;
}

static kv_t *bstselect(void *symtbl, int k)
{
	bst_symtbl_t *st = (bst_symtbl_t *)symtbl;
	symnode_t *m;

	m = _select(st->root, k);

	return m->value;
}

void *new_symtbl(int bforce, type_kv tkey)
{
	bst_symtbl_t *st;
	
	st = (bst_symtbl_t *)CALLOC(1, sizeof(bst_symtbl_t));

	st->root = NULL;
	
	st->put = bstput;
	st->get = bstget;
	st->rank = bstrank;
	st->min = bstmin;
	st->floor = bstfloor;
	st->select = bstselect;
	st->free = free_symtbl;
	st->del = bstdel;
	st->size = bstsize;

	st->bforce= bforce;
	st->tkey = tkey;

	return st;
}


