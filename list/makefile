CFLAGS = -ggdb -Wall
CC = gcc
LDFLAGS = -lm
OBJS = main.o testframe.o listsymtbl.o syscallpack.o rolist.o kvnode.o
KEYTYPE = 4
TESTNUM = 1000
STRATEGY = list

lst: $(OBJS)
	$(CC) -o $@ $(OBJS)

main.o: main.c listsymtbl.h testframe.h
	$(CC) $(CFLAGS) -c -o $@ $<

testframe.o: testframe.c listsymtbl.c listsymtbl.h syscallpack.h kvnode.h testframe.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
listsymtbl.o: listsymtbl.c listsymtbl.h syscallpack.h kvnode.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
rolist.o: rolist.c rolist.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
syscallpack.o: syscallpack.c syscallpack.h rodebug.h
	$(CC) $(CFLAGS) -c -o $@ $<

kvnode.o: kvnode.c kvnode.h compobject.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -fr *.o lst inputdata outputdata

run:
	valgrind --db-attach=yes --tool=memcheck ./lst

test:
	awk -f gendata.awk $(TESTNUM) $(KEYTYPE)
	awk   -v tkey=$(KEYTYPE) -v strategy=$(STRATEGY) -f gentest.awk inputdata 
	make 
	./lst > outputdata
	
	