#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

#define PORT 8081

int main() {
    int sockfd;
    char buffer[1024];
    struct sockaddr_in servaddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        return 1;
    }

    std::cout << "UDP server listening on port " << PORT << std::endl;

    socklen_t len = sizeof(cliaddr);
    int n = recvfrom(sockfd, (char *)buffer, 1024, 0, (struct sockaddr *) &cliaddr, &len);
    buffer[n] = '\0';
    std::cout << "Client: " << buffer << std::endl;

    const char *hello = "Hello from UDP Server!";
    sendto(sockfd, hello, strlen(hello), 0, (const struct sockaddr *) &cliaddr, len);

    close(sockfd);
    return 0;
}