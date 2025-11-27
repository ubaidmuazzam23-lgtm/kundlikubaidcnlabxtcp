#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        return 1;
    }

    // Bind socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        return 1;
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    // Accept a connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        return 1;
    }

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, 1024);
        if (valread <= 0) {
            std::cout << "Client disconnected.\n";
            break;
        }
        std::cout << "Client: " << buffer << "\n";

        // Auto reply
        std::string reply = "Server received: " + std::string(buffer);
        send(new_socket, reply.c_str(), reply.size(), 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}