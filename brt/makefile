CFLAGS = -ggdb -Wall
CC = gcc
LDFLAGS = -lm
OBJS = main.o testframe.o brtsymtbl.o syscallpack.o kvnode.o
KEYTYPE = 2
TESTNUM = 1000	
STRATEGY = brt

brt: $(OBJS)
	$(CC) -o $@ $(OBJS)

main.o: main.c brtsymtbl.h
	$(CC) $(CFLAGS) -c -o $@ $<

testframe.o: testframe.c brtsymtbl.h syscallpack.h kvnode.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
brtsymtbl.o: brtsymtbl.c brtsymtbl.h syscallpack.h kvnode.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
syscallpack.o: syscallpack.c syscallpack.h rodebug.h
	$(CC) $(CFLAGS) -c -o $@ $<

kvnode.o: kvnode.c kvnode.h compobject.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -fr *.o brt inputdata outputdata

run:
	valgrind --db-attach=yes --tool=memcheck ./brt

test:
	awk -f gendata.awk $(TESTNUM) $(KEYTYPE)
	awk   -v tkey=$(KEYTYPE) -v strategy=$(STRATEGY) -f gentest.awk inputdata 
	make
	./brt > outputdata
	
	