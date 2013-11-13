SHELL = /bin/bash
.SUFFIXES:

CC = gcc
CFLAGS = -c -Wall -pedantic -ansi -std=c99 -fPIC -DDSFMT_MEXP=19937 
LDFLAGS = -lm

SOURCES = coherentmc.c a2c.c a4d.c dSFMT/dSFMT.c random.c
PREPROCESSED = $(SOURCES:.c=.i)
OBJECTS = $(SOURCES:.c=.o)
SHAREDOBJECT = coherentmc.so


coherentmc: $(SOURCES) $(SHAREDOBJECT)

$(SHAREDOBJECT): $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

dSFMT/dSFMT.o: dSFMT/dSFMT.c
	$(CC) $(CFLAGS) -O3 -DHAVE_SSE2 $< -o $@


preprocess: $(PREPROCESSED)

%.i: %.c
	$(CC) $(CFLAGS) $< -E -o $@


.PHONY: clean
clean:
	rm -f $(OBJECTS) $(PREPROCESSED) $(SHAREDOBJECT)

