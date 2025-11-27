#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define PORT 9090

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

    std::string filename = "sample.txt";
    send(sock, filename.c_str(), filename.size(), 0);

    std::ofstream outfile("received_" + filename, std::ios::binary);
    char buffer[1024];
    ssize_t bytesRead;
    while ((bytesRead = read(sock, buffer, sizeof(buffer))) > 0) {
        outfile.write(buffer, bytesRead);
    }

    outfile.close();
    close(sock);
    return 0;
}