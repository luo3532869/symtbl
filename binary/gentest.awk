BEGIN {
		
		bforce = 1
		if (tkey == 1)
			keytype = "TKVINT"
		else if (tkey == 2)
			keytype = "TKVDOUBLE"
		else if (tkey == 3)
			keytype = "TKVSTRING"
		else
		{
			bforce = 0
			keytype = "TKVNULL"
		}
		nelems = 0

		
		tfile = "main.c"
		regstr = /^[0-9]+$/
		print "#include \"" strategy "symtbl.h\"" > tfile
		print "#include \"rodebug.h\"" > tfile
		print "#include \"compobject.h\"" > tfile
		print "#include <stdlib.h>" > tfile
		print "#include \"syscallpack.h\"" > tfile
		print "#include \"testframe.h\"" > tfile
		
		print "int main() {" > tfile
		print strategy "_symtbl_t *st = (" strategy "_symtbl_t *)new_symtbl(" bforce "," keytype ");" > tfile
		
}

{addput($1, $2) }

END		{
			addcheck()
			if (strategy != list)
				print "checkvalid(st);" > tfile
			print "st->free(st);" > tfile
			print "FREE(st);" > tfile
			print "return 0;" > tfile 
			print "}" > tfile
}
		
function isint(var)
{	
	if (var ~ /^[0-9]+$/)
		return 1
	else
		return 0
}

function isdouble(var)
{
	if (var ~ /^[+-]?([0-9]+[.]?[0-9]*|[.][0-9]+)([eE][+-]?[0-9]+)?$/)
		return 1
	else
		return 0
}

function isstring(var)
{
	if (var ~ /^[a-zA-Z_]+$/)
		return 1
	else 
		return 0
}

function gettypemacro(var)
{
	if (isint(var))
		return "_TI"
	else if (isdouble(var))
		return "_TD"
	else if (isstring(var))
		return "_TS"
	else 
		return "_TN"
}

function gettype(var)
{
	if (isint(var))
		return "TKVINT"
	else if (isdouble(var))
		return "TKVDOUBLE"
	else if (isstring(var))
		return "TKVSTRING"
	else 
		return "TKVNULL"
}

function getpvalue(var)
{
	if (isstring(var))
		return "\"" var "\""
	else
		return var
}

function getwildcard(var)
{
	if (isstring(var))
		return "s"
	else if (isint(var))
		return "d"
	else if (isdouble(var))
		return "g"
	else
		return "s"
}

/*get a random number from 1 to n*/
function randint(   n)
{
	return int(n *rand()) + 1
}

function arraylen(array, 	item, len)
{
	len = 0
	for (item in array)
		len++

	return len
}

function addput(key, value)
{
	rec[key] = value
	print "st->put(st,", gettypemacro(key) "(" getpvalue(key) "),",  gettypemacro(value) "(" getpvalue(value) "));" > tfile
	print "checklength(st," arraylen(rec)  ");" > tfile
}

function addcheck(	k, i)
{
	for (k in rec)
	{
		print "checkvalue(st,", gettype(k) ", "   getpvalue(k) ", ", gettype(rec[k]) ", " getpvalue(rec[k]) ");"  > tfile
		i = randint(10)

		if (i < 3)
		{
			print "checkdel(st,"  gettype(k)   ","   getpvalue(k)   ");"  > tfile
			delete rec[k]
			print "checklength(st," arraylen(rec)  ");" > tfile
		}	
	}
}


