//
// Created by Arthur on 22/11/2023.
// Host name resolution and connection establishment
// returns clientSocket
//

#include "../../includes/define.h"

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <netdb.h>
#endif

#define PORT 5555

int initConnexion() {
    // Initialize Winsock for Windows systems
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        error_content(500);
        return EXIT_FAILURE;
    }

    const char *hostname = settings->carName;
    struct addrinfo hints, *result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // Name resolution
    int res = getaddrinfo(hostname, NULL, &hints, &result);
    if (res != 0) {
        error_content(500);
        WSACleanup();
        return EXIT_FAILURE;
    }

    struct in_addr *address = &((struct sockaddr_in *) result->ai_addr)->sin_addr;
    const char *ip_address = inet_ntoa(*address);

    freeaddrinfo(result);

    // Create client socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        WSACleanup();
        error_content(500);
        return EXIT_FAILURE;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip_address);
    serverAddr.sin_port = htons(PORT);

    // Attempt to connect to the server
    if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        error_content(500);
        closesocket(clientSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    return clientSocket;
}
// Close socket and free Winsock for Windows systems
void closeConnexion(int clientSocket) {
    closesocket(clientSocket);
    WSACleanup();
}
