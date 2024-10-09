#ifndef SADMAN_RCON_H
#define SADMAN_RCON_H

#include <iostream>
#include <string>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif

class Rcon {
private:
    int rcon_sock;
    struct sockaddr_in server_addr;
    std::string rcon_password;

    Rcon() : rcon_sock(-1) {}

    void closeSocket() {
        if (rcon_sock >= 0) {
#ifdef _WIN32
            closesocket(rcon_sock);
#else
            ::close(rcon_sock);
#endif
            rcon_sock = -1;
        }
    }

public:
    static Rcon& getInstance() {
        static Rcon instance;
        return instance;
    }

    // Prevent copy constructor and assignment operator
    Rcon(const Rcon&) = delete;
    void operator=(const Rcon&) = delete;

#ifdef _WIN32
    // Initialize Winsock for Windows
    bool initWinsock() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Failed to initialize Winsock" << std::endl;
            return false;
        }
        return true;
    }

    // Clean up Winsock on Windows
    void cleanupWinsock() {
        WSACleanup();
    }
#endif

    // Connect to the Quake 3-based server with RCON password
    bool connect(const std::string& ip, int port, const std::string& password) {
#ifdef _WIN32
        if (!initWinsock()) {
            return false;
        }
#endif
        // Close the socket
        closeSocket();

        rcon_sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (rcon_sock < 0) {
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }

        // Set up server address structure
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);

#ifdef _WIN32 // Thank you chatgpt
        server_addr.sin_addr.s_addr = inet_addr(ip.c_str()); // Use inet_addr for Windows :D
#else
        inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr); // Use inet_pton for Linux and other shits
#endif

        rcon_password = password;

        return true;
    }

    std::string send(const std::string& command) {
        if (rcon_sock < 0) {
            return "Error: Not connected to any server.";
        }

        std::string rcon_command = "\xFF\xFF\xFF\xFF" + std::string("rcon ") + rcon_password + " " + command;

        sendto(rcon_sock, rcon_command.c_str(), rcon_command.size(), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        char buffer[4096];
        socklen_t addr_len = sizeof(server_addr);
        int recv_len = recvfrom(rcon_sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&server_addr, &addr_len);

        if (recv_len < 0) {
            return "Error receiving data.";
        }

        buffer[recv_len] = '\0';
        return std::string(buffer);
    }
};

auto& rcon = Rcon::getInstance();

#endif 
