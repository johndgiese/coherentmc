SHELL=/bin/sh
.SUFFIXES:

CC=gcc
CFLAGS=-c -Wall -pedantic -ansi -std=c99 -fPIC
LDFLAGS=-lm

SOURCES=coherentmc.c a2c.c a4d.c
PREPROCESSED=$(SOURCES:.c=.i)
OBJECTS=$(SOURCES:.c=.o)
SHAREDOBJECT=coherentmc.so


all: $(SOURCES) $(SHAREDOBJECT)

$(SHAREDOBJECT): $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@



preprocess: $(PREPROCESSED)

%.i: %.c
	$(CC) $(CFLAGS) $< -E -o $@


.PHONY: clean
clean:
	rm -f $(OBJECTS) $(PREPROCESSED) $(SHAREDOBJECT)

