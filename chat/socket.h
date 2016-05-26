#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8888
#define SIZE 2048

class Socket
{
public:
    Socket();
    Socket(char *ipaddr, int port);
    ~Socket();
    void setInfo(char *ipaddr, int port);
    int init();
    void end();
    int join();
    int exit();
    int send(char *ipaddr, char *port, char *msg);
    int recv(char *res);

private:
    int sockfd;
    struct sockaddr_in server;
    char *ipaddr;
    int port;
};

#endif // SOCKET_H
