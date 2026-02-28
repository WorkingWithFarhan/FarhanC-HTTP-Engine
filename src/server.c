#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

int main() {
    WSADATA wsa;
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // 1. Winsock start karo
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // 2. Socket create karo
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        printf("Socket failed\n");
        return 1;
    }

    // 3. Address setup
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 4. Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        return 1;
    }

    // 5. Listen
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        printf("Listen failed\n");
        return 1;
    }

    printf("Server chal raha hai port %d pe...\n", PORT);

    // 6. Accept
    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    if (new_socket == INVALID_SOCKET) {
        printf("Accept failed\n");
        return 1;
    }

    recv(new_socket, buffer, sizeof(buffer), 0);
    printf("Request:\n%s\n", buffer);

    char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Hello from Farhan Windows Server!";

    send(new_socket, response, strlen(response), 0);

    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();

    return 0;
}