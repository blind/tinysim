CC=gcc
CXX=clang++
CFLAGS= -O0 -g -std=c++11 -mmacosx-version-min=10.11 -Wextra -Wall -F/Library/Frameworks -I../include -I-../include
LIBS=-framework SDL2 -lc++ -lc
LDFLAGS= -lc++ -macosx_version_min 10.11 -framework SDL2


CFILES=tinysim.cpp spisim.cpp

OBJS=$(CFILES:.cpp=.o)

all: main.o tinysim.lib 
	$(LD) $(LDFLAGS) -o tinytest $(LIBS) $^

tinysim.lib: $(OBJS)
	$(AR) rcs tinysim.lib $(OBJS)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@  $<

#%.o: %.c
#	$(CXX) $(CFLAGS) -c -o $@ $<


clean:
	rm -f tinysim.lib tinytest $(OBJS) *.o

