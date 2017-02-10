CXX=g++
CXXFLAGS=-Wall -std=c++11
LIBS=

SOURCES=main.cc socket.cc

OBJECTS=$(SOURCES:.cc=.o)


all: ircbot

ircbot: $(OBJECTS)
	$(CXX) $^ -o $@ $(LIBS)

clean:
	rm -f *.o

distclean: clean
	rm -f ircbot

