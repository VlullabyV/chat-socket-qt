#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <iostream>
#include <string>

#define	PORT 8888
#define	SIZE 2048

std::map<std::string, std::string> ipmap;

std::string returnIpList() {
    std::string res = "";
    std::map<std::string ,std::string >::iterator it;
    for(it=ipmap.begin(); it!=ipmap.end(); ++it) {
        res += it->second;
        res += " ";
    }
    return res;
}

int main ( int argc, char *argv[] )
{
    int sockfd;
    sockfd = -1;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > sockfd) {
        fprintf(stderr, "error in creating socket\n");
        exit(-1);
    }
    int opt = SO_REUSEADDR;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    int port = PORT;
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    
    struct sockaddr_in client_from;
    
    struct sockaddr_in client_to;
    client_to.sin_family = AF_INET;
    
    unsigned int len = sizeof(struct sockaddr);
    int rvalue;
    rvalue = -1;
    rvalue = bind(sockfd, (struct sockaddr*)&server, len);
    if (0 > rvalue) {
        fprintf(stderr, "error in binding socket\n");
        close(sockfd);
        exit(-1);
    }
    
    char send_buf[SIZE];
    char recv_buf[SIZE];
    char fromip[20];
    int fromport;
    char fromportch[10];
    char toip[20];
    int toport;
    char toportch[10];
    char msg[SIZE];
    
    while(1) {
        
        memset(send_buf, 0, SIZE);
        memset(recv_buf, 0, SIZE);
        memset(fromip, 0, 20);
        memset(fromportch, 0, 10);
        memset(toip, 0, 20);
        memset(toportch, 0, 10);
        memset(msg, 0, SIZE);
  
        rvalue = -1;
        rvalue = recvfrom(sockfd, recv_buf, SIZE, 0, (struct sockaddr*)&client_from, &len);
        if (0 > rvalue) {
            fprintf(stderr, "error in receiving data\n");
            continue;
        }
        recv_buf[SIZE-1] = '\0';
        
        strcpy(fromip, inet_ntoa(client_from.sin_addr));
        fromport = ntohs(client_from.sin_port);
        sprintf(fromportch, "%d", fromport);
        char* token = strtok(recv_buf, ":");
        strcpy(toip, token);
        token = strtok(NULL, ":");
        strcpy(toportch, token);
        toport = atoi(toportch);
        token = strtok(NULL, ":");
        strcpy(msg, token);
        
        if (strcmp(toip, "1.1.1.1") == 0 && toport == 1000) {
            
            std::string tmp = "";
            tmp += fromip;
            tmp += ":";
            tmp += fromportch;
            
            if (!strcmp(msg, "join")) {
                ipmap.insert(std::map<std::string,std::string>::value_type(tmp, tmp));
            } else if (!strcmp(msg, "exit")) {
                std::map<std::string ,std::string >::iterator it;
                it=ipmap.find(tmp);
                if(it != ipmap.end())
                    ipmap.erase(it);
            }
            
            std::string list = "iplist:";
            list += returnIpList();
            strcpy(send_buf, list.c_str());
            
            std::string res, tmp1, tmp2;
            int pos;
            std::map<std::string ,std::string >::iterator it;
            for(it=ipmap.begin(); it!=ipmap.end(); ++it) {
                res = it->second;
                pos = (int)res.find(":");
                tmp1 = res.substr(0, pos);
                tmp2 = res.substr(pos+1);
                client_to.sin_addr.s_addr = inet_addr(tmp1.c_str());
                toport = atoi(tmp2.c_str());
                client_to.sin_port = htons(toport);
                sendto(sockfd, send_buf, SIZE, 0, (struct sockaddr*)&client_to, len);
            }
            
        } else {
            
            strcpy(send_buf, fromip);
            strcat(send_buf, ":");
            strcat(send_buf, fromportch);
            strcat(send_buf, ":");
            strcat(send_buf, msg);

            if (0 == strcmp(toip, "224.0.0.1")) {
                std::string res, tmp1, tmp2;
                int pos;
                std::map<std::string ,std::string >::iterator it;
                for(it=ipmap.begin(); it!=ipmap.end(); ++it) {
                    res = it->second;
                    pos = (int)res.find(":");
                    tmp1 = res.substr(0, pos);
                    tmp2 = res.substr(pos+1);
                    client_to.sin_addr.s_addr = inet_addr(tmp1.c_str());
                    toport = atoi(tmp2.c_str());
                    client_to.sin_port = htons(toport);
                    sendto(sockfd, send_buf, SIZE, 0, (struct sockaddr*)&client_to, len);
                }
            } else {
                client_to.sin_addr.s_addr = inet_addr(toip);
                client_to.sin_port = htons(toport);
                rvalue = -1;
                rvalue = sendto(sockfd, send_buf, SIZE, 0, (struct sockaddr*)&client_to, len);
                if (0 > rvalue) {
                    fprintf(stderr,"error in sending data\n");
                    continue;
                }
            }
        }
    }
    close(sockfd);
    return EXIT_SUCCESS;
}







