#include <array>
#include <string>
#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdlib>
#include <sys/wait.h>
#include <errno.h>
#include <chrono>
#include <set>

#include "FileOps.hpp"
#include "TextEditor.hpp"
#include "DataTypes.hpp"

using addrinfo = struct addrinfo;
using sockaddr_storage = struct sockaddr_storage;

void* get_in_addr(sockaddr* sa) {
    if(sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void bindServer(addrinfo* serverinfo, int& socketFD) {
    int yes {1};
    for(addrinfo* it {serverinfo}; it != NULL; it = it->ai_next) {
        if((socketFD = socket(it->ai_family, it->ai_socktype, it->ai_protocol)) == -1) {
            std::cout << "server: socket\n";
            continue;
        }

        if(setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            std::cout << "setsockopt\n";
            freeaddrinfo(serverinfo);
            std::abort();
        }

        if(bind(socketFD, it->ai_addr, it->ai_addrlen) == -1) {
            std::cout << "server: bind\n";
            continue;
        }

        if(it == NULL) {
            std::cout << "server failed to bind\n";
            freeaddrinfo(serverinfo);
            std::abort();
        }
        break;
    }
}

int main() {
    int socketFD, newFD;
    char hostname[100];

    int foo {gethostname(hostname, 100)};
    std::cout << hostname;

    addrinfo hints, *serverinfo;
    sockaddr_storage clientAddress;
    struct sigaction sa;
    socklen_t sin_size;

    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    rv = getaddrinfo(NULL, "7000", &hints, &serverinfo);
    bindServer(serverinfo, socketFD);
    
    freeaddrinfo(serverinfo);

    if(listen(socketFD, 10) == -1) {
        std::cout << "listen\n";
        std::abort();
    }

    std::cout << "waiting for connections...\n";

    sin_size = sizeof(clientAddress);
    newFD = accept(socketFD, reinterpret_cast<sockaddr*>(&clientAddress), &sin_size);
    if(newFD == -1) {
        std::cout << "accept\n";
        std::abort();
    }
    inet_ntop(clientAddress.ss_family, get_in_addr(reinterpret_cast<sockaddr*>(&clientAddress)), s, sizeof(s));
    std::cout << "got connection from " << s << '\n';

    //this is after connection, all that matters

    std::uint32_t bytes {};

    std::set<std::string, std::vector<better::Text>> buffers {};
    buffers.insert("foo.txt", {better::Text{better::openFile("/home/tim/foo.txt"),{0,0},{0,0}}})

    while(1) {
        std::uint64_t connected {recv(newFD, &bytes, sizeof(bytes), 0)};
        bytes = ntohl(bytes);
        better::DataIn command {bytes};
        connected = recv(newFD, command.data, command.size, 0);
        std::cout << "received: " << bytes << " bytes\n";
        std::string message {command.toParsedString()};
        std::cout << message << '\n';

        if(message == std::string("text-edit")) {
            connected = recv(newFD, &bytes, sizeof(bytes), 0);
            better::DataIn caretPos {ntohl(bytes)};
            connected = recv(newFD, caretPos.data, caretPos.size, 0);

            connected = recv(newFD, &bytes, sizeof(bytes), 0);
            better::DataIn anchorPos {ntohl(bytes)};
            connected = recv(newFD, anchorPos.data, anchorPos.size, 0);

            connected = recv(newFD, &bytes, sizeof(bytes), 0);
            better::DataIn keyTyped {ntohl(bytes)};
            connected = recv(newFD, keyTyped.data, keyTyped.size, 0);
            std::cout << "received " << ntohl(bytes) << " bytes\n";
            std::cout << keyTyped.toParsedString() << '\n';
            std::cout << caretPos.toString() << '\n';
            std::cout << anchorPos.toString() << '\n';
        }
        else if(message == std::string("get-directory")) {

        }
        else if(message == std::string("open-file")) {

        }
        else if(message == std::string("close-file")) {

        }
        else {

        }
    }
    
    close(socketFD);
    close(newFD);
    return 0;
}