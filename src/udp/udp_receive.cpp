#include "../database.h"

#include <iostream>

#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7501
#define REPLY_PORT 7500

int receive() {
	int sockfd;
    struct sockaddr_in my_addr;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    char buffer[1024];

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        std::cerr << "socket" << std::endl;
        exit(1);
    }

    // Fill in the server's sockaddr_in structure
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    // listen on all network interfaces
	my_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified address and port
    if (bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1) {
        std::cerr << "bind" << std::endl;
        exit(1);
    }

    std::cout << "Listening for UDP broadcasts on..." << PORT << std::endl;

    while (1) {
        // Receive data from clients
        ssize_t num_bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_len);
        if (num_bytes == -1) {
            std::cerr << "recvfrom" << std::endl;
            exit(1);
        }

        // Print the received data
        buffer[num_bytes] = '\0'; // Null-terminate the received data
        std::cout << "Received from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << ":" << buffer << std::endl;

        // Parse "shooter:target" and reply with the equipment id that was hit
        int shooter, target;
        if (sscanf(buffer, "%d:%d", &shooter, &target) == 2) {
            char reply[16];
            snprintf(reply, sizeof(reply), "%d", target);

            struct sockaddr_in reply_addr = client_addr;
            reply_addr.sin_port = htons(REPLY_PORT);

            if (sendto(sockfd, reply, strlen(reply), 0,
                       (struct sockaddr*)&reply_addr, sizeof(reply_addr)) == -1) {
                std::cerr << "sendto" << std::endl;
            } else {
                std::cout << "Replied with: " << reply << std::endl;
            }

            // friendly fire detection
			if (Database::friendlyFire(shooter, target)) {
				snprintf(reply, sizeof(reply), "%d", shooter);
				struct sockaddr_in reply_addr = client_addr;
				reply_addr.sin_port = htons(REPLY_PORT);

				if (sendto(sockfd, reply, strlen(reply), 0,
						   (struct sockaddr*)&reply_addr, sizeof(reply_addr)) == -1) {
					std::cerr << "sendto" << std::endl;
				} else {
					std::cout << "Replied with: " << reply << std::endl;
				}
			}
        }
    }

    // Close the socket
    close(sockfd);

    return 0;
}
