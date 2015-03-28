#include "brtsymtbl.h"
#include "rodebug.h"
#include <stdlib.h>
#include "syscallpack.h"
#include "testframe.h"
int main() {
brt_symtbl_t *st = (brt_symtbl_t *)new_symtbl(1,TKVINT);
st->put(st, _TI(5), _TI(29433));
checklength(st,1);
st->put(st, _TI(12), _TD(6.02375e+08));
checklength(st,2);
st->put(st, _TI(8), _TI(57542));
checklength(st,3);
st->put(st, _TI(25), _TS("fdykwtpjbxfLdxuCz_ZLUpCc"));
checklength(st,4);
st->put(st, _TI(4), _TD(3.73448e+08));

st->put(st, _TI(32), _TD(3.73448e+08));
st->put(st, _TI(17), _TD(3.73448e+08));

checkvalid(st);
displaysymtbl(st);

st->delmax(st);
checkvalid(st);
displaysymtbl(st);

st->delmax(st);
checkvalid(st);
displaysymtbl(st);

dispaytreeheight(st);
st->free(st);
FREE(st);
return 0;
}


