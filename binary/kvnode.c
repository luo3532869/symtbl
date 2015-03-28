/*
	File:	kvnode.c
	Author: Robin
	Time: 2015/03/13 15:58
*/
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "kvnode.h"
#include "rodebug.h"
#include "compobject.h"
#include "syscallpack.h"

#define KVPRINT(fmt, arg...) 	printf(fmt"", ##arg)
 
struct t_ops
{
	type_kv t;
	kvops_t ops;
};

void null_print(kv_t *kv)
{
	assert (TKVNULL == kv->t);
	KVPRINT("%s", "null");
}

int null_cmp(kv_t *k1, kv_t *k2)
{
	return 0;
}

void int_print(kv_t *kv)
{
	assert (TKVINT == kv->t);
	KVPRINT("%d", _DI(kv));
}

int int_cmp(kv_t *k1, kv_t *k2)
{
	assert(k1->t == k2->t);
	assert(TKVINT == k1->t);

	return _DI(k1) - _DI(k2);
}

void double_print(kv_t *kv)
{
	assert (TKVDOUBLE== kv->t);
	KVPRINT("%g", _DD(kv));
}

int double_cmp(kv_t *k1, kv_t *k2)
{
	double d;
	assert(k1->t == k2->t);
	assert(TKVDOUBLE == k1->t);

	d = _DD(k1) - _DD(k2);
	if (d > 0)
		return 1;
	else if (d < 0)
		return -1;
	else
		return 0;
}

void string_print(kv_t *kv)
{
	assert (TKVSTRING== kv->t);
	KVPRINT("%s", _DS(kv));
}

void string_free(kv_t *kv)
{
	assert (TKVSTRING== kv->t);
	FREE(_DS(kv));
}

int string_cmp(kv_t *k1, kv_t *k2)
{
	assert(k1->t == k2->t);
	assert (TKVSTRING== k1->t);

	return strcmp(_DS(k1), _DS(k2));
}

void object_print(kv_t *kv)
{
	assert (TKVOBJECT== kv->t);
	kvobj_t *kvobj;
	kvobj = _DO(kv);
	if (NULL != kvobj->print)
	{
		kvobj->print(kvobj);
	}
	else
		KVPRINT("object: %08x", (unsigned int)kvobj);
}

void object_free(kv_t *kv)
{
	assert (TKVOBJECT== kv->t);
	kvobj_t *kvobj;
	kvobj = _DO(kv);
	
	if (NULL != kvobj->free)
	{
		kvobj->free(kvobj);
	}

	FREE(kvobj);
}

int object_cmp(kv_t *k1, kv_t *k2)
{
	assert(k1->t == k2->t);
	assert (TKVOBJECT== k1->t);

	kvobj_t *o1;
	kvobj_t *o2;
	o1 = _DO(k1);
	o2 = _DO(k2);
	
	if (NULL != o1->cmp)
	{
		return o1->cmp(o1, o2);
	}
	else		
		return _DO(k1) - _DO(k2);
}

struct t_ops allops[] = 
{
	{TKVNULL, {null_print, NULL, null_cmp}},
	{TKVINT, {int_print, NULL, int_cmp}},
	{TKVDOUBLE, {double_print, NULL, double_cmp}},
	{TKVSTRING, {string_print, string_free, string_cmp}},
	{TKVOBJECT, {object_print, object_free, object_cmp}},		/*default object operations,should register by user*/
	{TKVEND, {NULL, NULL, NULL}}
};


kv_t *newkvnode(type_kv t, va_list *arg)
{
	va_list ap = *arg;
	int n;
	double d;
	char *p;
	int size;
	
	kv_t *kv;
	kv = (kv_t *)CALLOC(1, sizeof(kv_t));
	
	kv->t = t;
	kv->ops = allops[t].ops;
	switch(t)
	{
		case TKVNULL:
			kv->o.dstring = "null";
			break;
		case TKVINT:
			n = va_arg(ap, int);
			kv->o.dint = n;
			break;
		case TKVDOUBLE:
			d = va_arg(ap, double);
			kv->o.ddouble = d;
			break;
		case TKVSTRING:
			p = va_arg(ap, char *);
			kv->o.dstring = STRDUP(p);
			break;
		case TKVOBJECT:
			p = va_arg(ap, void *);
			size = va_arg(ap, int);
			kv->o.dobj = MALLOC(size);
			Memcpy(kv->o.dobj, p, size);

			break;
		default:
			goto FAIL;
			break;
	}

	*arg = ap;
	return kv;
FAIL:
	FREE(kv);
	return NULL;
}

int freekvnode(kv_t *node)
{
	if (NULL != (_OPS(node)).free)
			(_OPS(node)).free(node);
	FREE(node);

	return 0;
}

