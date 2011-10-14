CC=g++
CFLAGS=-Wall
CTEMPLATE=-I$(HOME)/local/include -L$(HOME)/local/lib -lctemplate_nothreads
GLIBS=-lgflags -lglog
LIBEVENT=$(shell pkg-config --cflags --libs libevent)

all: server

mime.o: mime.cc mime.hpp
	$(CC) -c $<

server: server.cc mime.o
	$(CC) -g $(CFLAGS) $(CTEMPLATE) $(LIBEVENT) $(GLIBS) mime.o $< -o $@

tiny-server: server.cc mime.o
	$(CC) -Os $(CFLAGS) $(CTEMPLATE) $(LIBEVENT) $(GLIBS) mime.o $< -o $@
	-strip -s $@

clean:
	-rm -f *.o server tiny-server

.PHONY: all clean
