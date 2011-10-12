CC=g++
CFLAGS=-Wall
CTEMPLATE=-I$(HOME)/local/include -L$(HOME)/local/lib -lctemplate
BOOST_PO=-lboost_program_options
LIBEVENT=$(shell pkg-config --cflags --libs libevent)

all: server

server: server.cc
	$(CC) -g $(CFLAGS) $(CTEMPLATE) $(LIBEVENT) $(BOOST_PO) $< -o $@

tiny-server: server.cc
	$(CC) -Os $(CFLAGS) $(CTEMPLATE) $(LIBEVENT) $(BOOST_PO) $< -o $@
	-strip -s $@

clean:
	-rm -f *.o server tiny-server

.PHONY: all clean
