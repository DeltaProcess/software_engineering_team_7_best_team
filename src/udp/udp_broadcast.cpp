#include <iostream>

#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7500
#define NETWORK_ADDRESS "127.0.0.1"

int broadcast(const char* networkAddress) {
    int sockfd;
    struct sockaddr_in broadcast_addr;
    char broadcast_message[] = "202";

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        std::cout << "unable to create socket" << std::endl;
        exit(1);
    }

    // Set socket options to allow broadcast
    int broadcast_enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) == -1 ) {
        std::cout << "unable to enable broadcasts" << std::endl;
        exit(1);
    }

    // Initialize the broadcast address structure
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, networkAddress, &broadcast_addr.sin_addr) <= 0) {
        std::cerr << "inet_pton" << std::endl;
        exit(1);
    }

    // Send the broadcast message
    ssize_t bytes_sent = sendto(sockfd, broadcast_message, strlen(broadcast_message), 0,
                                (struct sockaddr*)&broadcast_addr, sizeof(broadcast_addr));
    
    if (bytes_sent == -1) {
        std::cerr << "sendto" << std::endl;
    } else {
        std::cout << "sent " << bytes_sent << " bytes to " << networkAddress << ":" << PORT << std::endl;
    }

    close(sockfd);
    return 0;
}
