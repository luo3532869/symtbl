CFLAGS = -ggdb -Wall
CC = gcc
LDFLAGS = -lm
OBJS = main.o testframe.o bstsymtbl.o syscallpack.o kvnode.o
KEYTYPE = 3
TESTNUM = 10000
STRATEGY = bst

bst: $(OBJS)
	$(CC) -o $@ $(OBJS)

main.o: main.c bstsymtbl.h
	$(CC) $(CFLAGS) -c -o $@ $<

testframe.o: testframe.c bstsymtbl.h syscallpack.h kvnode.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
bstsymtbl.o: bstsymtbl.c bstsymtbl.h syscallpack.h kvnode.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
syscallpack.o: syscallpack.c syscallpack.h rodebug.h
	$(CC) $(CFLAGS) -c -o $@ $<

kvnode.o: kvnode.c kvnode.h compobject.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -fr *.o bst inputdata outputdata

run:
	valgrind --db-attach=yes --tool=memcheck ./bst

test:
	awk -f gendata.awk $(TESTNUM) $(KEYTYPE)
	awk   -v tkey=$(KEYTYPE) -v strategy=$(STRATEGY) -f gentest.awk inputdata 
	make
	./bst > outputdata
	
	