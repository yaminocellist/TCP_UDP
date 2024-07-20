#include "socket.h"

// Function to obfuscate and deobfuscate data using XOR cipher
void xor_cipher(char* data, size_t length, const char* key, size_t key_length) {
    for (size_t i = 0; i < length; ++i) {
        data[i] ^= key[i % key_length];
    }
}

int main() {
    const char* key = "mysecretkey"; // Key for XOR cipher
    size_t key_length = strlen(key);

    int udp_sock = create_udp_socket();
    if (udp_sock < 0) {
        std::cerr << "Failed to create UDP socket!" << std::endl;
        return -1;
    }

    struct sockaddr_in udp_serv_addr, udp_client_addr;
    init_sockaddr_in(udp_serv_addr, nullptr, UDP_PORT);

    if (bind(udp_sock, (const struct sockaddr *)&udp_serv_addr, sizeof(udp_serv_addr)) < 0) {
        std::cerr << "Binding UDP socket error!" << std::endl;
        close(udp_sock);
        return -1;
    }

    std::cout << "UDP2TCP is running. Waiting for UDP packets on port " << UDP_PORT << "..." << std::endl;

    // Uncomment and adjust TCP socket creation when testing with a TCP server
    // int tcp_sock = create_tcp_socket("127.0.0.1", TCP_PORT);
    // if (tcp_sock < 0) {
    //     close(udp_sock);
    //     return -1;
    // }

    socklen_t addr_len = sizeof(udp_client_addr);
    while (true) {
        ssize_t received_bytes = recvfrom(udp_sock, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&udp_client_addr, &addr_len);
        if (received_bytes < 0) {
            std::cerr << "Receiving UDP packet error!" << std::endl;
            continue;
        }

        buffer[received_bytes] = '\0'; // Null-terminate the received data
        std::cout << "Received UDP data: " << buffer << std::endl;

        if (strcmp(buffer, "quit") == 0) {
            std::cout << "Received quit command, shutting down..." << std::endl;
            break;
        }

        // Print raw data before obfuscation
        std::cout << "Raw data before obfuscation (hex): ";
        for (size_t i = 0; i < received_bytes; ++i) {
            std::cout << std::hex << (int)(unsigned char)buffer[i] << ' ';
        }
        std::cout << std::endl;

        // Obfuscate the data
        xor_cipher(buffer, received_bytes, key, key_length);

        // Print obfuscated data as hex to handle non-printable characters
        std::cout << "Obfuscated data (hex): ";
        for (size_t i = 0; i < received_bytes; ++i) {
            std::cout << std::hex << (int)(unsigned char)buffer[i] << ' ';
        }
        std::cout << std::endl;

        // Ensure null-termination after obfuscation
        buffer[received_bytes] = '\0';

        // Print obfuscated data as a string, may not display correctly if non-printable
        std::cout << "Obfuscated data (string): ";
        for (size_t i = 0; i < received_bytes; ++i) {
            std::cout << (isprint(buffer[i]) ? buffer[i] : '.');
        }
        std::cout << std::endl;

        // Uncomment when TCP server is available
        // ssize_t sent_bytes = send(tcp_sock, buffer, received_bytes, 0);
        // if (sent_bytes < 0) {
        //     std::cerr << "Sending data to TCP server error!" << std::endl;
        //     continue;
        // }

        std::cout << "Obfuscated data forwarded from UDP to TCP..." << std::endl;
    }

    // Uncomment when TCP server is available
    // close(tcp_sock);
    close(udp_sock);

    return 0;
}
