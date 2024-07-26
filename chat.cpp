#include "chat.h"
#include <iostream>
#include <cstring>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>

char nickname[50];
int port;
char ip[16];
int sock;
sockaddr_in broadcast_addr;

void* receive_messages(void* arg) {
    char buffer[1024];
    sockaddr_in sender_addr;
    socklen_t addr_len = sizeof(sender_addr);

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&sender_addr, &addr_len);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0'; // Ensure the string is null-terminated
            char sender_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &sender_addr.sin_addr, sender_ip, INET_ADDRSTRLEN);
            std::cout << "Message from " << sender_ip << ": " << buffer << std::endl;
        }
    }
    return nullptr;
}

void* send_messages(void* arg) {
    char message[1000];
    char buffer[1050];

    while (true) {
        std::cin.getline(message, sizeof(message));
        
        memset(buffer, 0, sizeof(buffer));

        // Properly use snprintf considering the buffer size
        int len = snprintf(buffer, sizeof(buffer), "%s: %s", nickname, message);
        if (len < 0 || len >= static_cast<int>(sizeof(buffer))) { // Cast sizeof(buffer) to int
            std::cerr << "Message too long to send" << std::endl;
        } else {
            sendto(sock, buffer, strlen(buffer), 0, (sockaddr*)&broadcast_addr, sizeof(broadcast_addr));
        }
    }
    return nullptr;
}

int setup_socket() {
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    int broadcast_enable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0) {
        perror("setsockopt (SO_BROADCAST)");
        close(sock);
        return 1;
    }

    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(port);
    inet_pton(AF_INET, "255.255.255.255", &broadcast_addr.sin_addr);

    sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &local_addr.sin_addr);

    if (bind(sock, (sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
        perror("bind");
        close(sock);
        return 1;
    }
    return 0;
}

void get_user_input() {
    std::cout << "Enter your nickname: ";
    std::cin.getline(nickname, sizeof(nickname));
}
