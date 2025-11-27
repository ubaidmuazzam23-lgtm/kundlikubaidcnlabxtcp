#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    std::cout << "Connected to server. Type messages (type 'exit' to quit):\n";

    while (true) {
        std::string msg;
        std::cout << "You: ";
        std::getline(std::cin, msg);

        send(sock, msg.c_str(), msg.size(), 0);

        if (msg == "exit") break;

        memset(buffer, 0, sizeof(buffer));
        int valread = read(sock, buffer, sizeof(buffer));
        if (valread > 0) {
            std::cout << "Server: " << buffer << std::endl;
        }
    }

    close(sock);
    return 0;
}


// xcode-select --install
// g++ udp_calc_client.cpp -o udp_calc_client && ./udp_calc_client
// g++ udp_calc_server.cpp -o udp_calc_server && ./udp_calc_server


// g++ udp_client.cpp -o udp_client && ./udp_client
//g++ udp_server.cpp -o udp_server && ./udp_server

// g++ hello_server.cpp -o hello_server && ./hello_server
// g++ hello_client.cpp -o hello_client && ./hello_client

// g++ file_server.cpp -o file_server && ./file_server
//g++ file_client.cpp -o file_client && ./file_client

