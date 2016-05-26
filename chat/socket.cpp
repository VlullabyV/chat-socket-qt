#include "socket.h"

Socket::Socket() {
    Socket::sockfd = -1;
    Socket::ipaddr = new char[20];
    Socket::port = 0;
}

Socket::Socket(char *ipaddr, int port) {
    Socket::sockfd = -1;
    Socket::port = port;
    Socket::ipaddr = new char[20];
    strcpy(Socket::ipaddr, ipaddr);
}

Socket::~Socket() {

}

void Socket::setInfo(char *ipaddr, int port) {
    strcpy(Socket::ipaddr, ipaddr);
    Socket::port = port;
}

int Socket::init() {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > sockfd) {
        return 0;
    }
    int opt = SO_REUSEADDR;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ipaddr);
    /*
    int rvalue;
    rvalue = -1;
    rvalue = bind(sockfd, (struct sockaddr*)&server, sizeof(struct sockaddr));
    if (0 > rvalue) {
        close(sockfd);
        return 0;
    }
    */
    return 1;
}

void Socket::end() {
    close(sockfd);
}

int Socket::join() {
    char toipaddr[20];
    char toport[10];
    char msg[SIZE];
    strcpy(toipaddr, "1.1.1.1");
    strcpy(toport, "1000");
    strcpy(msg, "join");
    int rvalue = -1;
    rvalue = send(toipaddr, toport, msg);
    if (0 > rvalue) {
        return 0;
    }
    return 1;
}

int Socket::exit() {
    char toipaddr[20];
    char toport[10];
    char msg[SIZE];
    strcpy(toipaddr, "1.1.1.1");
    strcpy(toport, "1000");
    strcpy(msg, "exit");
    int rvalue = -1;
    rvalue = send(toipaddr, toport, msg);
    if (0 > rvalue) {
        return 0;
    }
    return 1;
}

int Socket::send(char *toipaddr, char *toport, char *msg) {
    char send_buf[SIZE];
    memset(send_buf, 0, SIZE);
    strcpy(send_buf, toipaddr);
    strcat(send_buf, ":");
    strcat(send_buf, toport);
    strcat(send_buf, ":");
    strcat(send_buf, msg);
    int rvalue = -1;
    rvalue = sendto(sockfd, send_buf, SIZE, 0, (struct sockaddr*)&server, sizeof(struct sockaddr));
    if (0 > rvalue) {
        return 0;
    }
    return 1;
}

int Socket::recv(char *res) {
    char recv_buf[SIZE];
    memset(recv_buf, 0, SIZE);
    unsigned int len = sizeof(struct sockaddr);
    int rvalue = -1;
    rvalue = recvfrom(sockfd, recv_buf, SIZE, 0, (struct sockaddr*)&server, &len);
    if (0 > rvalue) {
        return 0;
    }
    recv_buf[SIZE-1] = '\0';
    strcpy(res, recv_buf);
    return 1;
}
