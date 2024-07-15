#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <algorithm>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Create socket file descriptor:
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))  == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080:
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Binding:
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // x)
    std::cout << "==================================\n          Server Started:\n==================================" << std::endl;

    // Listen:
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accept a connection:
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    

    // Loop to keep receiving commands:
    while(true) {
        memset(buffer, 0, sizeof(buffer));  // clear the buffer;
        int valread = read(new_socket, buffer, 1024);
        if (valread <= 0) {
            break;
        }
        std::cout << "Command received: \n" << buffer << std::endl;

        // Convert buffer to uppercase for comparison
        std::string command(buffer);
        std::transform(command.begin(), command.end(), command.begin(), ::toupper);

        // "QUIT" makes it quit:
        if (command == "QUIT") {
            std::cout << "Received QUIT command. Stopping server." << std::endl;
            break;
        }
    }

    close(new_socket);
    close(server_fd);

    return 0;
}