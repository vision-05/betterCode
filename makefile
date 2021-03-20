CC := g++

CXXFLAGS := -g -Ofast -std=c++20

SERVERSRCS = Server/DataTypes.cpp Server/TextEditor.cpp Server/FileOps.cpp
SERVERTESTSRCS = $(SERVERSRCS) Server/Test.cpp
SERVERAPPSRCS = $(SERVERSRCS) Server/TextServer.cpp
SERVERSRCDIRS = $(shell find . -nmae '*.cpp' | dirname {} | uniq | sed 's/\/$(SERVERDIR)//g')

EXEC := BetterCode

TESTEXEC := BetterTestServer

DESTDIR := /usr/local
PREFIX := debug

EXECDIR := bin
OBJDIR := bin
INCLUDE := /usr/include/c++/10.2.0/immer-0.6.2/ /usr/include/c++/10.2.0/
SERVERDIR := Server

Server: $(EXEC)
$(EXEC): $(SERVERDIR)/*.cpp makefile
	mkdir -p $(EXECDIR)/$(PREFIX)/$(SERVERDIR)
	$(CC) -o $(EXECDIR)/$(PREFIX)/$(SERVERDIR)/$@ $(CXXFLAGS) $(SERVERAPPSRCS) $(INCLUDE:%=-I%)

ServerTest: $(TESTEXEC)
$(TESTEXEC): $(SERVERDIR)/*.cpp makefile
	mkdir -p $(EXECDIR)/test/$(SERVERDIR)
	$(CC) -o $(EXECDIR)/test/$(SERVERDIR)/$@ $(CXXFLAGS) $(SERVERTESTSRCS) $(INCLUDE:%=-I%) -lgtest

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
