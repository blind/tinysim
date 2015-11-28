CC=gcc
CFLAGS= -O0 -g -std=c11 -F/Library/Frameworks
LIBS=-framework SDL2


CFILES=tinysim.c

OBJS=$(CFILES:.c=.o)

all: tinysim.lib
	$(CC) $(CFLAGS) -o tinytest main.c tinysim.lib $(LIBS)

tinysim.lib: $(OBJS)
	$(AR) rcs tinysim.lib $(OBJS)

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<



clean:
	rm -f tinysim.lib tinytest $(OBJS)