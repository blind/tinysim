CC=gcc
CXX=clang++
#CFLAGS= -O0 -g -std=c++11 -mmacosx-version-min=10.11 -Wextra -Wall -F/Library/Frameworks -I../include -I-../include
CFLAGS= -O0 -g -std=c++11 -mmacosx-version-min=10.9 -F/Library/Frameworks -I../include -I-../include
LIBS=-framework SDL2 -lc++ -lc
LDFLAGS=-macosx_version_min 10.9


CFILES=tinysim.cpp spisim.cpp simtinyscreen.cpp simwire.cpp

OBJS=$(CFILES:.cpp=.o)

all: tinysim.lib tinytest

tinytest: test.o tinysim.lib
	$(LD) $(LDFLAGS) -o tinytest $(LIBS) $^

tinysim.lib: $(OBJS)
	$(AR) rcs tinysim.lib $(OBJS)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@  $<

clean:
	rm -f tinysim.lib tinytest $(OBJS) *.o

.PHONY: all
