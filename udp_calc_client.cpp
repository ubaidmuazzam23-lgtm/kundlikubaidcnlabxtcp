#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

#define PORT 8082

int main() {
    int sockfd;
    char buffer[1024];
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    std::string input;
    std::cout << "Enter operation and angle (e.g. sin 30): ";
    std::getline(std::cin, input);

    sendto(sockfd, input.c_str(), input.length(), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));

    socklen_t len = sizeof(servaddr);
    int n = recvfrom(sockfd, (char *)buffer, 1024, 0, (struct sockaddr *) &servaddr, &len);
    buffer[n] = '\0';

    std::cout << "Server: " << buffer << std::endl;

    close(sockfd);
    return 0;
}