CC := g++

CXXFLAGS := -g -Ofast -std=c++20

SERVERSRCS = src/Server/DataTypes.cpp src/Server/TextEditor.cpp src/Server/FileOps.cpp src/Server/TextServer.cpp
SERVERSRCDIRS = $(shell find . -nmae '*.cpp' | dirname {} | uniq | sed 's/\/$(SRCDIR)/$(SERVERDIR)//g')

CLIENTSRCS = src/Server/GuiClient.cpp
CLIENTSRCDIRS = $(shell find . -nmae '*.cpp' | dirname {} | uniq | sed 's/\/$(SRCDIR)/$(CLIENTDIR)//g')

EXEC := BetterCode
CLIENTEXEC := BetterCodeClient

DESTDIR := /usr/local
PREFIX := debug

EXECDIR := bin
OBJDIR := bin
SRCDIR := src
INCLUDE := /usr/local/include/c++/11.0.0/immer-0.6.2/ /usr/local/include/c++/11.0.0/
SERVERDIR := Server
CLIENTDIR := Client

Server: $(EXEC)
$(EXEC): $(SRCDIR)/$(SERVERDIR)/*.cpp makefile
	mkdir -p $(EXECDIR)/$(PREFIX)/$(SERVERDIR)
	$(CC) -o $(EXECDIR)/$(PREFIX)/$(SERVERDIR)/$@ $(CXXFLAGS) $(SERVERSRCS) $(INCLUDE:%=-I%)

Client: $(CLIENTEXEC)
$(CLIENTEXEC): $(SRCDIR)/$(CLIENTDIR)/*.cpp makefile
	mkdir -p $(EXECDIR)/$(PREFIX)/$(CLIENTDIR)
	$(CC) -o $(EXECDIR)/$(PREFIX)/$(CLIENTDIR)/$@ $(CXXFLAGS) $(CLIENTSRCS) $(INCLUDE:%=-I%)


.PHONY: install
install: $(EXEC)
	mkdir -p $(DESTDIR)/$(PREFIX)/bin
	cp $< $(DESTDIR)/$(PREFIX)/bin/$(EXEC)

.PHONY: uninstall
uninstall:
	rm $(DESTDIR)/$(PREFIX)/bin/$(EXEC)

.PHONY: clean
clean:
	rm -f $(EXECDIR)/$(PREFIX)/$(SERVERDIR)/$(EXEC) $(EXECDIR)/$(PREFIX)/$(CLIENTDIR)/$CLIENTEXEC $(SERVEROBJS) $(CLIENTOBJS)
