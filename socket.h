#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

#define UDP_PORT    9090
#define TCP_PORT    8080
#define BUFFER_SIZE 1024

extern char buffer[BUFFER_SIZE];

int create_udp_socket();
int create_tcp_socket(const char* ip, int port);
void init_sockaddr_in(struct sockaddr_in& addr, const char* ip, int port, int family = AF_INET);

#endif