# var
MODULE = $(notdir $(CURDIR))
OS     = $(shell uname -s)
NOW    = $(shell date +%d%m%y)
REL    = $(shell git rev-parse --short=4 HEAD)
BRANCH = $(shell git rev-parse --abbrev-ref HEAD)

# tool
CURL = curl -L -o
CF   = clang-format

# src
C  += src/$(MODULE).cpp
H  += inc/$(MODULE).hpp
F  += lib/$(MODULE).ini
S  += $(C) $(H) $(F)
CP += tmp/$(MODULE).parser.cpp tmp/$(MODULE).lexer.cpp
HP += tmp/$(MODULE).parser.hpp

# cfg
CFLAGS += -pipe -O0 -g2 -Iinc -Itmp -std=c++17

# all
all: bin/$(MODULE) $(F)
	$^

# format
format: tmp/format_cpp
tmp/format_cpp: $(C) $(H)
	$(CF) --style=file -i $? && touch $@

# rule
bin/$(MODULE): $(C) $(H) $(CP) $(HP)
	$(CXX) $(CFLAGS) -o $@ $(C) $(CP)

tmp/$(MODULE).parser.cpp: src/$(MODULE).yacc
	bison -o $@ $<
tmp/$(MODULE).lexer.cpp: src/$(MODULE).lex
	flex -o $@ $<

.PHONY: doc
doc: doc/Starting-FORTH.pdf doc/thinking-forth-color.pdf
	rsync -rv ~/mdoc/pmem/*      doc/pmem/
	rsync -rv ~/mdoc/$(MODULE)/* doc/$(MODULE)/
	git add doc

doc/Starting-FORTH.pdf:
	$(CURL) $@ https://www.forth.com/wp-content/uploads/2018/01/Starting-FORTH.pdf
doc/thinking-forth-color.pdf:
	$(CURL) $@ http://prdownloads.sourceforge.net/thinking-forth/thinking-forth-color.pdf

# doc
doxy: .doxygen
	rm -rf docs ; doxygen $< 1>/dev/null

# install
install: doc gz
	$(MAKE) update
update:
	sudo apt update
	sudo apt install -yu `cat apt.txt`
	$(MAKE) src

gz: src

.PHONY: src
src:

# merge
MERGE  = Makefile README.md .clang-format .doxygen $(S)
MERGE += apt.txt
MERGE += .vscode bin doc lib inc src tmp

dev:
	git push -v
	git checkout $@
	git pull -v
	git checkout shadow -- $(MERGE)
	$(MAKE) doxy ; git add -f docs

shadow:
	git push -v
	git checkout $@
	git pull -v

release:
	git tag $(NOW)-$(REL)
	git push -v --tags
	$(MAKE) shadow

ZIP = tmp/$(MODULE)_$(NOW)_$(REL)_$(BRANCH).zip
zip:
	git archive --format zip --output $(ZIP) HEAD
