#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define PORT 8888
#define SIZE 2048

class Socket
{
public:
    Socket();
    ~Socket();
    int init(char *ipaddr, int port);
    int initbind(int multiport);
    void end();
    int join();
    int exit();
    int send(char *ipaddr, char *port, char *msg);
    int recv(char *res);
    void joinGroup(char *multiaddr);
    void exitGroup(char *multiaddr);

private:
    int sockfd;
    struct sockaddr_in server;
};

#endif // SOCKET_H
