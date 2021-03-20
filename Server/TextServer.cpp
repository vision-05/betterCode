#include <array>
#include <string>
#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdlib>
#include <sys/wait.h>
#include <errno.h>

#include "FileOps.hpp"
#include "TextEditor.hpp"
#include "DataTypes.hpp"

using addrinfo = struct addrinfo;
using sockaddr_storage = struct sockaddr_storage;

void sigchld_handler(int s) {
    int savedErrno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = savedErrno;
}

void* get_in_addr(sockaddr* sa) {
    if(sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
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

    int yes {1};
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    rv = getaddrinfo(NULL, "7000", &hints, &serverinfo);
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
    
    freeaddrinfo(serverinfo);

    if(listen(socketFD, 10) == -1) {
        std::cout << "listen\n";
        std::abort();
    }

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL) == -1) {
        std::cout << "sigaction\n";
        std::abort();
    }

    std::cout << "waiting for connections...\n";

    while(1) {
        sin_size = sizeof(clientAddress);
        newFD = accept(socketFD, reinterpret_cast<sockaddr*>(&clientAddress), &sin_size);
        if(newFD == -1) {
            std::cout << "accept\n";
            continue;
        }

        inet_ntop(clientAddress.ss_family, get_in_addr(reinterpret_cast<sockaddr*>(&clientAddress)), s, sizeof(s));
        std::cout << "got connection from " << s << '\n';

        if(!fork()) {
            close(socketFD);
            if(send(newFD, "Hello, World!", 13, 0) == -1) {
                std::cout << "send\n";
            }
            close(newFD);
            break;
        }
        close(newFD);
    }

    return 0;
}