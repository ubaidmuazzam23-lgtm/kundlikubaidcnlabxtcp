#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <cmath>

#define PORT 8082

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

    std::cout << "UDP Calculator server listening on port " << PORT << std::endl;

    socklen_t len = sizeof(cliaddr);

    while (true) {
        int n = recvfrom(sockfd, (char *)buffer, 1024, 0, (struct sockaddr *) &cliaddr, &len);
        if (n < 0) break;
        buffer[n] = '\0';

        std::string request(buffer);
        // Expect input like: "sin 30" or "cos 45" or "tan 60"
        std::string op;
        double angle_deg;
        sscanf(buffer, "%s %lf", &op[0], &angle_deg);

        double angle_rad = angle_deg * M_PI / 180.0;
        double result = 0;

        if (request.find("sin") == 0) {
            result = sin(angle_rad);
        } else if (request.find("cos") == 0) {
            result = cos(angle_rad);
        } else if (request.find("tan") == 0) {
            result = tan(angle_rad);
        } else {
            std::string err = "Invalid operation";
            sendto(sockfd, err.c_str(), err.length(), 0, (struct sockaddr *)&cliaddr, len);
            continue;
        }

        std::string reply = "Result: " + std::to_string(result);
        sendto(sockfd, reply.c_str(), reply.length(), 0, (struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);
    return 0;
}