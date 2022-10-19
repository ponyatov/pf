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

L += -lpmem

# cfg
CFLAGS += -pipe -O0 -g2 -Iinc -Itmp -std=c++17

# all
all: bin/$(MODULE) $(F)
	$^ && hexdump -C tmp/$(MODULE).pmem

# format
format: tmp/format_cpp
tmp/format_cpp: $(C) $(H)
	$(CF) --style=file -i $? && touch $@

# rule
bin/$(MODULE): $(C) $(H) $(CP) $(HP) tmp/.gitignore
	$(CXX) $(CFLAGS) -o $@ $(C) $(CP) $(L)

tmp/$(MODULE).parser.cpp: src/$(MODULE).yacc
	bison -o $@ $<
tmp/$(MODULE).lexer.cpp: src/$(MODULE).lex
	flex -o $@ $<

tmp/.gitignore:
	git checkout tmp

.PHONY: doc
doc: \
	doc/Starting-FORTH.pdf doc/thinking-forth-color.pdf \
	doc/pmem.pdf \
	doc/ProjectOberon1992.pdf
	rsync -rv ~/mdoc/pmem/*      doc/pmem/
	rsync -rv ~/mdoc/FORTH/*     doc/FORTH/
	rsync -rv ~/mdoc/$(MODULE)/* doc/$(MODULE)/
	git add doc

doc/Starting-FORTH.pdf:
	$(CURL) $@ https://www.forth.com/wp-content/uploads/2018/01/Starting-FORTH.pdf
doc/thinking-forth-color.pdf:
	$(CURL) $@ http://prdownloads.sourceforge.net/thinking-forth/thinking-forth-color.pdf

doc/pmem.pdf:
	$(CURL) $@ https://library.oapen.org/bitstream/id/e234e601-6128-4ee4-be45-32e8f2e417dd/1007325.pdf

doc/ProjectOberon1992.pdf:
	$(CURL) $@ https://people.inf.ethz.ch/wirth/ProjectOberon1992.pdf
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
	$(MAKE) doc doxy ; git add -f docs

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

NPM = $(HOME)/.npm
vscode:
#	npm install -g --prefix=$(NPM) yo generator-code
	rm -rf vscode ; cd vscode ; $(NPM)/bin/yo code
