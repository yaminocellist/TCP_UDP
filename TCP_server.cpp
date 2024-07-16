#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <algorithm>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>

#define TCP_PORT 8080
#define UDP_PORT 9090

void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;

    int len;
    int ciphertext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    unsigned char *key = (unsigned char *)"01234567890123456789012345678901"; // 256-bit key
    unsigned char *iv = (unsigned char *)"0123456789012345";                  // 128-bit IV


    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))  == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons(TCP_PORT);

    // Binding:
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen:
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accept connection from UDP:
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Create socket for UDP:
    int udp_sockfd;
    struct sockaddr_in udp_servaddr;

    if ((udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("UDP socket craetion failed");
        exit(EXIT_FAILURE);
    }

    memset(&udp_servaddr, 0, sizeof(udp_servaddr));
    udp_servaddr.sin_family      = AF_INET;
    udp_servaddr.sin_port        = htons(UDP_PORT);
    udp_servaddr.sin_addr.s_addr = INADDR_ANY;

    while(true) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, 1024);
        if (valread <= 0)   break;
        std::cout << "Data received: " << buffer << std::endl;

        unsigned char ciphertext[128];
        int ciphertext_len = encrypt((unsigned char *)buffer, strlen(buffer), key, iv, ciphertext);

        // Forward data to UDP server:
        sendto(udp_sockfd, ciphertext, ciphertext_len, 0, (const struct sockaddr *)&udp_servaddr, sizeof(udp_servaddr));
        // sendto(udp_sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&udp_servaddr, sizeof(udp_servaddr));
        std::cout << "Forwarded to UDP" << std::endl;

        // "QUIT" to quit the program:
        if (strcmp(buffer, "quit") == 0) {
            std::cout << "Received QUIT command. Stopping server." << std::endl;
            break;
        }
    }

    close(new_socket);
    close(server_fd);
    close(udp_sockfd);

    return 0;
}