#include "socket.h"

Socket::Socket() {
    Socket::sockfd = -1;
}

Socket::~Socket() {

}

//根据ip地址和端口号初始化
int Socket::init(char *ipaddr, int port) {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > sockfd) {
        return 0;
    }
    int opt = SO_REUSEADDR;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ipaddr);
    return 1;
}

//根据多播地址初始化
int Socket::initbind(int multiport) {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > sockfd) {
        return 0;
    }
    int opt = SO_REUSEADDR;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    server.sin_family = AF_INET;
    server.sin_port = htons(multiport);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    int rvalue = -1;
    rvalue = bind(sockfd, (struct sockaddr*)&server, sizeof(struct sockaddr));
    if (0 > rvalue) {
        close(sockfd);
        return 0;
    }
    return 1;
}

//注销
void Socket::end() {
    close(sockfd);
}

//向服务器发送上线消息
int Socket::join() {
    char toipaddr[20];
    char toport[10];
    char msg[SIZE];
    strcpy(toipaddr, "1.1.1.1");
    strcpy(toport, "10000");
    strcpy(msg, "join");
    int rvalue = -1;
    rvalue = send(toipaddr, toport, msg);
    if (0 > rvalue) {
        return 0;
    }
    return 1;
}

//向服务器发送下线消息
int Socket::exit() {
    char toipaddr[20];
    char toport[10];
    char msg[SIZE];
    strcpy(toipaddr, "1.1.1.1");
    strcpy(toport, "10000");
    strcpy(msg, "exit");
    int rvalue = -1;
    rvalue = send(toipaddr, toport, msg);
    if (0 > rvalue) {
        return 0;
    }
    return 1;
}

//向指定ip地址和端口号发送消息
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

//接收消息
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

//加入指定多播地址群组
void Socket::joinGroup(char *multiaddr) {
    struct ip_mreq ipmr;
    ipmr.imr_interface.s_addr = htonl(INADDR_ANY);
    ipmr.imr_multiaddr.s_addr = inet_addr(multiaddr);
    setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &ipmr, sizeof(ipmr));
}

//退出指定多播地址群组
void Socket::exitGroup(char *multiaddr) {
    struct ip_mreq ipmr;
    ipmr.imr_multiaddr.s_addr = inet_addr(multiaddr);
    setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &ipmr, sizeof(ipmr));
}
