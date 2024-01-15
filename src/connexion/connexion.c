#include "../../includes/define.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <netdb.h>
#endif

#define PORT 5555

int initConnexion() {
    // Initialiser Winsock pour les systèmes Windows
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        perror("Erreur lors de l'initialisation de Winsock");
        return EXIT_FAILURE;
    }

    const char *hostname = settings->carName;
    struct addrinfo hints, *result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // Résolution de nom
    int res = getaddrinfo(hostname, NULL, &hints, &result);
    if (res != 0) {
        fprintf(stderr, "Erreur lors de la resolution de nom: %s\n", gai_strerror(res));
        WSACleanup();
        return EXIT_FAILURE;
    }

    struct in_addr *address = &((struct sockaddr_in *) result->ai_addr)->sin_addr;
    const char *ip_address = inet_ntoa(*address);

    freeaddrinfo(result);

    // Création de la socket client
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Erreur lors de la création de la socket client");
        WSACleanup();
        return EXIT_FAILURE;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip_address);
    serverAddr.sin_port = htons(PORT);

    // Tentative de connexion au serveur
    if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        error_content(500);
        closesocket(clientSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    return clientSocket;
}

void closeConnexion(int clientSocket) {
    // Fermer la socket
    closesocket(clientSocket);

    // Libérer Winsock pour les systèmes Windows
    WSACleanup();
}
