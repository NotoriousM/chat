#include "chat.h"
#include <iostream>
#include <cstring>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <IPv4 address> <port>" << std::endl;
        return 1;
    }

    strncpy(ip, argv[1], sizeof(ip) - 1);
    port = atoi(argv[2]);

    get_user_input();

    if (setup_socket() != 0) {
        return 1;
    }

    pthread_t receiver_thread, sender_thread;

    if (pthread_create(&receiver_thread, nullptr, receive_messages, nullptr) != 0) {
        perror("pthread_create (receiver)");
        close(sock);
        return 1;
    }

    if (pthread_create(&sender_thread, nullptr, send_messages, nullptr) != 0) {
        perror("pthread_create (sender)");
        close(sock);
        return 1;
    }

    pthread_join(receiver_thread, nullptr);
    pthread_join(sender_thread, nullptr);

    close(sock);
    return 0;
}
