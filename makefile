SHELL = /bin/bash
.SUFFIXES:

MAKE = make
SUBPROJECTS = coherentmc


all: $(SUBPROJECTS)

.PHONY: $(SUBPROJECTS)
$(SUBPROJECTS):
	$(MAKE) -C $@


.PHONY: clean
clean:
	for PROJ in $(SUBPROJECTS); do \
		$(MAKE) -C $$PROJ clean; \
	done

