CC=g++
CFLAGS=-Wall
CTEMPLATE=-I$(HOME)/local/include -L$(HOME)/local/lib -lctemplate_nothreads
GLIBS=-lgflags -lglog
LIBEVENT=$(shell pkg-config --cflags --libs libevent)

all: server

server: server.cc
	$(CC) -g $(CFLAGS) $(CTEMPLATE) $(LIBEVENT) $(GLIBS) $< -o $@

tiny-server: server.cc
	$(CC) -Os $(CFLAGS) $(CTEMPLATE) $(LIBEVENT) $(GLIBS) $< -o $@
	-strip -s $@

clean:
	-rm -f *.o server tiny-server

.PHONY: all clean
