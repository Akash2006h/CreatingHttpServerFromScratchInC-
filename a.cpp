#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char* argv[])
{
    addrinfo hints{}, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    if (argc != 2) {
        std::cerr << "usage: showip hostname\n";
        return 1;
    }

    // Set up hints
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;      // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP

    // Resolve hostname
    status = getaddrinfo(argv[1], nullptr, &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(status) << "\n";
        return 2;
    }

    std::cout << "IP addresses for " << argv[1] << ":\n\n";

    // Loop through results
    for (p = res; p != nullptr; p = p->ai_next) {
        void* addr;
        const char* ipver;

        if (p->ai_family == AF_INET) { // IPv4
            auto* ipv4 = reinterpret_cast<sockaddr_in*>(p->ai_addr);
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            auto* ipv6 = reinterpret_cast<sockaddr_in6*>(p->ai_addr);
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // Convert IP to string
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        std::cout << "  " << ipver << ": " << ipstr << "\n";
    }

    freeaddrinfo(res); // Free memory
    return 0;
}
