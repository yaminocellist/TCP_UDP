#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sockfd;                 // socket file descriptor;
    char buffer[1024];
    struct sockaddr_in servaddr;
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Creating socket failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    socklen_t len;
    int n;
    
    while (true) {
        std::cout << "Please enter the command, in forms of\n<HEX_Address>+<COMMAND>:\n";
        std::cin.getline(buffer, sizeof(buffer));
        
        sendto(sockfd, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        std::cout << "Command sent.\n";
        
        // Check for a quit command to quit
        if (strcmp(buffer, "quit") == 0) {
            break;
        }

        // Receive response from UDP_server:
        n = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
        buffer[n] = '\0';
        std::cout << "Server: " << buffer << std::endl;
    }
    
    close(sockfd);
    return 0;
}
