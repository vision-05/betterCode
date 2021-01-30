CC := clang++-10

CXXFLAGS := -g -Ofast -std=c++20
LLDLIBS := -lSDL2main -lSDL2

SRCS = $(shell find $(SRCDIR) -name '*.cpp')
SRCDIRS = $(shell find . -nmae '*.cpp' | dirname {} | uniq | sed 's/\/$(SRCDIR)//g')
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

EXEC := BetterCode

DESTDIR := /usr/local
PREFIX := debug

EXECDIR := bin
OBJDIR := bin
SRCDIR := src
INCLUDE := /usr/include/c++/10/immer-0.6.2/

all: $(EXEC)
$(EXEC): $(OBJS) $(HDRS) makefile
	mkdir -p $(EXECDIR)/$(PREFIX)
	$(CC) -o $(EXECDIR)/$(PREFIX)/$@ $(OBJS) $(CXXFLAGS) $(LLDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(OPTS) -c $< $(INCLUDE:%=-I%) -std=c++20 -o $@

.PHONY: install
install: $(EXEC)
	mkdir -p $(DESTDIR)/$(PREFIX)/bin
	cp $< $(DESTDIR)/$(PREFIX)/bin/$(EXEC)

.PHONY: uninstall
uninstall:
	rm $(DESTDIR)/$(PREFIX)/bin/$(EXEC)

.PHONY: clean
clean:
	rm -f $(EXECDIR)/$(PREFIX)/$(EXEC) $(OBJS)
