#include "socket.h"

char buffer[BUFFER_SIZE] = {0};

int create_udp_socket() {
    // Creating UDP socket:
    int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0) {
        std::cerr << "Creating UDP socket error!" << std::endl;
        return -1;
    }

    return udp_sock;
}

void init_sockaddr_in (struct sockaddr_in& addr, const char* ip, int port, int family) {
    addr.sin_family = family;
    addr.sin_port   = htons(port);
    if (ip == nullptr) {
        addr.sin_addr.s_addr = INADDR_ANY;
    } else {
        addr.sin_addr.s_addr = inet_addr(ip);
    }
}

int create_tcp_socket(const char* ip, int port) {
    int tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sock < 0) {
        std::cerr << "Creating TCP socket error!" << std::endl;
        return -1;
    }

    struct sockaddr_in serv_addr;
    init_sockaddr_in(serv_addr, ip, port);
    if (connect(tcp_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection to TCP server failed!" << std::endl;
        return -1;
    }

    return tcp_sock;
}