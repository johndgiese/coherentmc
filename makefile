SHELL = /bin/bash
.SUFFIXES:

MAKE = make
SUBPROJECTS = coherentmc

all: $(SUBPROJECTS)

cmc.py: cmcutils.py coherentmc.so

.PHONY: $(SUBPROJECTS)
$(SUBPROJECTS):
	$(MAKE) -C $@


FIGURE_SCRIPTS = figures/*.py

$(FIGURE_SCRIPTS): cmc.py

figures: $(FIGURE_SCRIPTS:.py=.pdf)

figures/%.pdf: figure_scripts/%.py
	python $<


.PHONY: clean
clean:
	for PROJ in $(SUBPROJECTS); do \
		$(MAKE) -C $$PROJ clean; \
	done
	rm figures/*

