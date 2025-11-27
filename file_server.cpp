#include <iostream>
#include <fstream>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>

#define PORT 9090

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024];

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

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        return 1;
    }

    std::cout << "Waiting for file request on port " << PORT << std::endl;

    // Accept connection
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        return 1;
    }

    // Receive requested filename
    ssize_t valread = read(new_socket, buffer, sizeof(buffer) - 1);
    if (valread <= 0) {
        std::cerr << "Failed to read filename" << std::endl;
        close(new_socket);
        close(server_fd);
        return 1;
    }

    buffer[valread] = '\0'; // Null-terminate
    std::string filename(buffer);

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "File not found: " << filename << std::endl;
        const char* msg = "ERROR: file not found";
        send(new_socket, msg, strlen(msg), 0);
        close(new_socket);
        close(server_fd);
        return 1;
    }

    // Send file contents
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        std::streamsize bytes_read = file.gcount();
        if (bytes_read > 0) {
            send(new_socket, buffer, bytes_read, 0);
        }
    }

    std::cout << "File sent successfully.\n";

    file.close();
    close(new_socket);
    close(server_fd);
    return 0;
}