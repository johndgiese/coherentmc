SHELL = /bin/bash
.SUFFIXES:

CC = clang
CFLAGS = -c -Wall -O3 -pedantic -ansi -std=c99 -fPIC -DDSFMT_MEXP=19937 
LDFLAGS = -lm

SOURCES = coherentmc.c a2c.c a4d.c dSFMT/dSFMT.c randutils.c
PREPROCESSED = $(SOURCES:.c=.i)
OBJECTS = $(SOURCES:.c=.o)
SHAREDOBJECT = coherentmc.so


coherentmc: $(SOURCES) $(SHAREDOBJECT)

$(SHAREDOBJECT): $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

dSFMT/dSFMT.o: dSFMT/dSFMT.c
	$(CC) $(CFLAGS) -DHAVE_SSE2 $< -o $@

randutils.so: $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS) $(LDFLAGS)


preprocess: $(PREPROCESSED)

%.i: %.c
	$(CC) $(CFLAGS) $< -E -o $@


.PHONY: clean
clean:
	rm -f $(OBJECTS) $(PREPROCESSED) $(SHAREDOBJECT) crandom.so dSFMT/dSFMT.o

