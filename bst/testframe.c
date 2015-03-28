/*
	File:	syscallpack.c
	Author: Robin
	Time: 2015/03/20 15:18
*/

#include "bstsymtbl.h"
#include "rodebug.h"
#include "compobject.h"
#include "syscallpack.h"
#include "kvnode.h"
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>

typedef struct _stu_t
{
	kvobj_t obj;
	int sno;
	char *name;
}stu_t;
 
int cmp_stu(void *o1, void *o2)
{
	stu_t *s1, *s2;

	assert(NULL != o1 && NULL != o2);

	s1 = (stu_t *)o1;
	s2 = (stu_t *)o2;

	return s1->sno - s2->sno;
 }
 
void print_stu(void *s1)
{
	assert(NULL != s1);
	stu_t *s;
	s = (stu_t *)s1;
	printf("sno:%d, name:%s\n", s->sno, s->name);
	return ;
}

void free_stu(void *s)
{
	#if 0
	if (NULL != s)
		free(s);
	#endif
}

void _checkbst(symnode_t *t)
{
	symnode_t *l, *r;

	if (NULL == t)
		return;
		
	if (NULL != (l = t->left))
	{
		assert(_OPS(l->key).cmp(l->key, t->key) <= 0);
	}

	if (NULL != (r = t->right))
	{
		assert(_OPS(r->key).cmp(r->key, t->key) >= 0);
	}

	_checkbst(t->left);
	_checkbst(t->right);
}

int checkvalid(void *symtbl)
{
	bst_symtbl_t *st = (bst_symtbl_t *)symtbl;

	_checkbst(st->root);
	return 0;
	
}

void _displaybst(symnode_t *t)
{
	kv_t *key;
	kv_t *value;
	
	if (NULL == t)
		return;

	_displaybst(t->left);
	
	key = t->key;
	_OPS(key).print(key);
	printf(":");
	value = t->value;
	_OPS(value).print(value);
	printf("\n");

	_displaybst(t->right);
}

void displaysymtbl(void *symtbl)
{
	bst_symtbl_t *st = (bst_symtbl_t *)symtbl;
	
	_displaybst(st->root);
}


void checkvalue(void *symtbl, type_kv tkv, ...)
{
	va_list ap;
	int n;
	double d;
	char *p;
	bst_symtbl_t *st = (bst_symtbl_t *)symtbl;
	
	va_start(ap, tkv);
	kv_t  *kv;
	
	switch(tkv)
	{
		case TKVINT:
			n = va_arg(ap, int);
			kv = st->get(st,  _TI(n));
			break;
		case TKVDOUBLE:
			d = va_arg(ap, double);
			kv = st->get(st,  _TD(d));
			break;
		case TKVSTRING:
			p = va_arg(ap, char *);
			kv = st->get(st,  _TS(p));
			break;
		case TKVOBJECT:
		case TKVNULL:
		default:
			ROHERE("not supported type in test.");
			exit(1);
			break;
	}

	tkv = va_arg(ap, int);
	assert(kv->t == tkv);
	
	switch(tkv)
	{
		case TKVNULL:
			break;
		case TKVINT:
			n = va_arg(ap, int);
			break;
		case TKVDOUBLE:
			d = va_arg(ap, double);
			break;
		case TKVSTRING:
			p = va_arg(ap, char *);
			break;
		case TKVOBJECT:
		default:
			ROHERE("not supported type in test.");
			exit(1);
			break;
	}

	if (NULL == kv && TKVNULL == tkv)
		return;

	switch(tkv)
	{
		case TKVNULL:
			break;
		case TKVINT:
			assert(kv->o.dint == n);
			break;
		case TKVDOUBLE:
			assert(kv->o.ddouble == d);
			break;
		case TKVSTRING:
			assert(0 == strcmp(kv->o.dstring, p));
			break;
		case TKVOBJECT:
		default:
			ROHERE("not supported type in test.");
			exit(1);
			break;
	}

	return;
}

void checklength(void *symtbl, int nelems)
{
	bst_symtbl_t *st = (bst_symtbl_t *)symtbl;

	assert(st->size(st) == nelems);
}

void checkdel(void *symtbl, type_kv t, ...)
{
	va_list ap;
	int n;
	double d;
	char *p;
	bst_symtbl_t *st = (bst_symtbl_t *)symtbl;
	
	va_start(ap, t);
	
	switch(t)
	{
		case TKVINT:
			n = va_arg(ap, int);
			st->del(st,  _TI(n));
			assert(NULL == st->get(st, _TI(n)));
			break;
		case TKVDOUBLE:
			d = va_arg(ap, double);
			st->del(st,  _TD(d));
			assert(NULL == st->get(st, _TD(d)));
			break;
		case TKVSTRING:
			p = va_arg(ap, char *);
			st->del(st,  _TS(p));
			assert(NULL == st->get(st, _TS(p)));
			break;
		case TKVOBJECT:
		case TKVNULL:
		default:
			ROHERE("not supported type in test.");
			exit(1);
			break;
	}
	
}
