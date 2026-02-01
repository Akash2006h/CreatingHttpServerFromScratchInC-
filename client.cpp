#include<iostream>
#include<netdb.h>
#include<sys/socket.h>
#include<cstring>

int main(){
    struct addrinfo hints{}, *res;
    int sock_fd;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, "3490", &hints, &res);
    sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sock_fd, res->ai_addr, res->ai_addrlen);
    listen(sock_fd, 10);
    struct sockaddr_storage client;
    socklen_t addr_size = sizeof(client);
    int new_fd = accept(sock_fd, (struct sockaddr *)&client, &addr_size);
    if(new_fd == -1){
        std::cerr<<"Failed"<<std::endl;
    }
    else{
        std::cout<<"Conection Establised";
    }
}