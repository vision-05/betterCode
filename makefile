CC := g++

CXXFLAGS := -g -Ofast -std=c++20

SERVERSRCS = src/Server/DataTypes.cpp src/Server/TextEditor.cpp src/Server/FileOps.cpp
SERVERTESTSRCS = $(SERVERSRCS) src/Server/Test.cpp
SERVERAPPSRCS = $(SERVERSRCS) src/Server/TextServer.cpp
SERVERSRCDIRS = $(shell find . -nmae '*.cpp' | dirname {} | uniq | sed 's/\/$(SRCDIR)/$(SERVERDIR)//g')

CLIENTSRCS = src/Server/GuiClient.cpp
CLIENTTESTSRCS = $(CLIENTSRCS) src/Client/Test.cpp
CLIENTAPPSRCS = $(CLIENTSRCS)
CLIENTSRCDIRS = $(shell find . -nmae '*.cpp' | dirname {} | uniq | sed 's/\/$(SRCDIR)/$(CLIENTDIR)//g')

EXEC := BetterCode
CLIENTEXEC := BetterCodeClient

TESTEXEC := BetterTestServer
TESTEXECCLIENT := BetterTestClient

DESTDIR := /usr/local
PREFIX := debug

EXECDIR := bin
OBJDIR := bin
SRCDIR := src
INCLUDE := /usr/include/c++/10.2.0/immer-0.6.2/ /usr/include/c++/10.2.0/
SERVERDIR := Server
CLIENTDIR := Client

Server: $(EXEC)
$(EXEC): $(SRCDIR)/$(SERVERDIR)/*.cpp makefile
	mkdir -p $(EXECDIR)/$(PREFIX)/$(SERVERDIR)
	$(CC) -o $(EXECDIR)/$(PREFIX)/$(SERVERDIR)/$@ $(CXXFLAGS) $(SERVERAPPSRCS) $(INCLUDE:%=-I%)

Client: $(CLIENTEXEC)
$(CLIENTEXEC): $(SRCDIR)/$(CLIENTDIR)/*.cpp makefile
	mkdir -p $(EXECDIR)/$(PREFIX)/$(CLIENTDIR)
	$(CC) -o $(EXECDIR)/$(PREFIX)/$(CLIENTDIR)/$@ $(CXXFLAGS) $(CLIENTAPPSRCS) $(INCLUDE:%=-I%)

ServerTest: $(TESTEXEC)
$(TESTEXEC): $(SRCDIR)/$(SERVERDIR)/*.cpp makefile
	mkdir -p $(EXECDIR)/test/$(SERVERDIR)
	$(CC) -o $(EXECDIR)/test/$(SERVERDIR)/$@ $(CXXFLAGS) $(SERVERTESTSRCS) $(INCLUDE:%=-I%) -lgtest

ClientTest: $(TESTEXECCLIENT)
$(TESTEXECCLIENT): $(SRCDIR)/$(CLIENTDIR)/*.cpp makefile
	mkdir -p $(EXECDIR)/test/$(CLIENTDIR)
	$(CC) -o $(EXECDIR)/test/$(CLIENTDIR)/$@ $(CXXFLAGS) $(CLIENTTESTSRCS) $(INCLUDE:%=-I%) -lgtest

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
