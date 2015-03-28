CFLAGS = -ggdb -Wall
CC = gcc
LDFLAGS = -lm
OBJS = main.o testframe.o binarysymtbl.o syscallpack.o kvnode.o
KEYTYPE = 1
TESTNUM = 1000
STRATEGY = binary

binary: $(OBJS)
	$(CC) -o $@ $(OBJS)

main.o: main.c binarysymtbl.h
	$(CC) $(CFLAGS) -c -o $@ $<

testframe.o: testframe.c binarysymtbl.c binarysymtbl.h syscallpack.h kvnode.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
binarysymtbl.o: binarysymtbl.c binarysymtbl.h syscallpack.h kvnode.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
syscallpack.o: syscallpack.c syscallpack.h rodebug.h
	$(CC) $(CFLAGS) -c -o $@ $<

kvnode.o: kvnode.c kvnode.h compobject.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -fr *.o binary inputdata outputdata

run:
	valgrind --db-attach=yes --tool=memcheck ./binary

test:
	awk -f gendata.awk $(TESTNUM) $(KEYTYPE)
	awk   -v tkey=$(KEYTYPE) -v strategy=$(STRATEGY) -f gentest.awk inputdata 
	make
	./binary > outputdata
	
	