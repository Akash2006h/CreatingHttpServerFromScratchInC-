#include<iostream>
#include<cstring>
#include<unistd.h>
#include<netdb.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#define PORT "8080"
#define BACKLOG 10
int main(){
    struct addrinfo hints{}, *servinfo, *p;
    int sockfd, new_fd;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    int yes = 1;

    if(getaddrinfo(NULL, PORT, &hints, &servinfo) != 0) {
        perror("getaddrinfo");
        return 1;
    }
    for(p = servinfo; p != NULL; p = p->ai_next){
        sockfd = socket(p->ai_family, p ->ai_socktype, p->ai_protocol);
        if(sockfd == -1) continue;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, & yes, sizeof yes);
        if(bind(sockfd,p->ai_addr, p->ai_addrlen) == 0) break;
        close(sockfd);
    }
    freeaddrinfo(servinfo);
    if(p == NULL){
        std:: cerr<<"Failed to bind"<<std :: endl;
        return 1;
    } 
    listen(sockfd, BACKLOG);
    std:: cout<<"Server is running in port 8080"<<std :: endl;
    while(true) {
        new_fd = accept(sockfd, NULL, NULL);
        if(new_fd == -1) continue;
        char buffer[4096] = {0};
        read(new_fd, buffer, sizeof(buffer));
        const char *body = "Hello from c++ server";
         std::string response = "HTTP/1.1 200 OK\r\n"
        "CONTENT-TYPE: text/plain\r\n"
        "CONTENT-LENGTH: " + std:: to_string(strlen(body)) + "\r\n" "\r\n" + std:: string(body);
        write(new_fd, response.c_str(), response.size());
        close(new_fd);
    }
    
}