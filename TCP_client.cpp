#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Creating socket:
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error!" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Conver IPv4 addresses fromt text to binary:
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address / Address not supported" << std::endl;
        return -1;
    }

    // Connecting to the server:
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed!" << std::endl;
        return -1;
    }

    while (true) {
        std::cout << "Please enter the command, in forms of\n<HEX_Address>+<COMMAND>:\n";
        std::cin.getline(buffer, sizeof(buffer));

        // send data:
        send(sock, buffer, strlen(buffer), 0);
        std::cout << "Comand sent\n";

        // Check for a quit command to quit:
        if (strcmp(buffer, "quit") == 0) {
            break;
        }
    }
    close(sock);

    return 0;
}