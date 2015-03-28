/*
	File:	testframe.c
	Author: Robin
	Time: 2015/03/25 09:56
*/

#include "listsymtbl.h"
#include "rodebug.h"
#include "compobject.h"
#include "syscallpack.h"
#include "kvnode.h"
#include "testframe.h"
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>

void checkvalue(void *symtbl, type_kv tkv, ...)
{
	va_list ap;
	int n;
	double d;
	char *p;
	list_symtbl_t *st = (list_symtbl_t *)symtbl;
	
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
	list_symtbl_t *st = (list_symtbl_t *)symtbl;

	assert(st->size(st) == nelems);
}

void checkdel(void *symtbl, type_kv t, ...)
{
	va_list ap;
	int n;
	double d;
	char *p;
	list_symtbl_t *st = (list_symtbl_t *)symtbl;
	
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
