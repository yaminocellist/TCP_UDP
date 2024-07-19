#include "socket.h"

int main () {
    int udp_sock = create_udp_socket();
    if (udp_sock < 0) return -1;

    struct sockaddr_in udp_serv_addr;
    init_sockaddr_in(udp_serv_addr, "127.0.0.1", UDP_PORT);

    while (true) {
        std::cout << "Please enter the command, in the form of <HEX_Address>+<COMMAND>:\n";
        std::cin.getline(buffer, BUFFER_SIZE);

        // Send data to UDP2TCP:
        sendto(udp_sock, buffer, strlen(buffer), 0, (const struct sockaddr *)&udp_serv_addr, sizeof(udp_serv_addr));
        std::cout << "Data sent to UDP2TCP..." << std::endl;

        // Check for quit:
        if (strcmp(buffer, "quit") == 0)   break;
    }

    close(udp_sock);

    return 0;
}