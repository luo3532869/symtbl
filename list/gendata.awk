BEGIN {
		ntype = split("int double string", types)
		split("a b c d e f g h i j k l m n o p q r s t u v w x y z A B C D E F G H I J K L M N O P Q R S T U V W X Y Z _", alphas)

		srand()
		
		if (ARGC < 3)
		{
			nitem = 100
			tkey = 1			#1 is int, 2 is double, 3 is string, 4 is random
		}
		else
		{
			nitem = ARGV[1]
			tkey = ARGV[2]
		}

		if (tkey > 3)
			randkey = 1
		else
			randkey = 0
		
		for (i = 0; i < nitem; i++)
		{
			if (randkey)
			{
				tkey = randint(3)
			}
			tval = randint(3)
			
			print randdata(tkey),  randdata(tval) > "inputdata"
		}
		
		
		exit
}

/*get a random number from 1 to n*/
function randint(   n)
{
	return int(n *rand()) + 1
}

function randdouble(  n)
{
	return (10 * n) * rand() * 0.1
}

function randstring(   i, j, len, string)
{
	len = randint(100)
	for (i = 0; i < len; i++)
	{
		j = randint(53)
		string = string alphas[j]
	}
	return string
}

function randdata(type)
{
	if (1 == type)
	{
		return randint(10000)
	}
	else if (2 == type)
		return randdouble(10000000000)
	else if (3 == type)
		return randstring()
}

