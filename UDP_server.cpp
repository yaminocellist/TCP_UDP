#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>

// #define PORT 8080
#define UDP_PORT 9090

int main() {
    int sockfd;                         // Socket File Descriptor;
    char buffer[1024];
    struct sockaddr_in servaddr, cliaddr;
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Creating socket failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr,  0, sizeof(cliaddr));
    
    servaddr.sin_family      = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port        = htons(UDP_PORT);
    
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("binding failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    socklen_t len;
    int n;
    
    len = sizeof(cliaddr); // len = value/result
    
    std::cout << "==================================\n         UDP Server Started:\n==================================" << std::endl;

    while (true) {
        memset(buffer, 0, sizeof(buffer));  // clear the buffer;
        
        n = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        std::cout << "Received from TCP: " << buffer << std::endl;
        
        // // Convert buffer to uppercase for comparison
        // std::string command(buffer);
        // std::transform(command.begin(), command.end(), command.begin(), ::toupper);
        
        // Manual to quit:
        if (strcmp(buffer, "QUIT") == 0) {
            std::cout << "Received QUIT command. Stopping server." << std::endl;
            break;
        }

        // // Respond to UDP_client:
        // const char *response = "Data received.";
        // sendto(sockfd, response, strlen(response), 0, (const struct sockaddr *)&cliaddr, len);
    }
    
    close(sockfd);
    return 0;
}
