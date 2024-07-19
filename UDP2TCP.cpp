#include <iostream>
#include <unistd.h>
#include <cstring>
#include "socket.h"

int main () {
    // Create and bind UDP socket:
    int udp_sock = create_udp_socket();
    if (udp_sock < 0)   return -1;

    struct sockaddr_in udp_serv_addr, cli_addr;
    init_sockaddr_in(udp_serv_addr, nullptr, UDP_PORT);

    if (bind(udp_sock, (const struct sockaddr *)&udp_serv_addr, sizeof(udp_serv_addr)) < 0) {
        std::cerr << "Binding UDP socket error!" << std::endl;
        return -1;
    }

    // Create and connect TCP socket:
    int tcp_sock = create_tcp_socket("127.0.0.1", TCP_PORT);
    if (tcp_sock < 0)   return -1;
    



}