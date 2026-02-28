/*
    FarhanC-HTTP-Engine
    --------------------
    Simple HTTP server built using Windows Winsock.

    Current Version: 0.2
    Goal:
    - Understand how socket server works
    - Handle one HTTP request
    - Send basic response

    Author: Farhan Khan
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {

    // ============================
    // Step 1: Initialize Winsock
    // ============================
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Winsock initialization failed.\n");
        return 1;
    }

    // ============================
    // Step 2: Create Socket
    // ============================
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    // ============================
    // Step 3: Configure Address
    // ============================
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;          // IPv4
    server_address.sin_addr.s_addr = INADDR_ANY;  // Accept any IP
    server_address.sin_port = htons(PORT);        // Convert port to network byte order

    // ============================
    // Step 4: Bind Socket to Port
    // ============================
    if (bind(server_socket, 
            (struct sockaddr *)&server_address, 
            sizeof(server_address)) == SOCKET_ERROR) {

        printf("Bind failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // ============================
    // Step 5: Start Listening
    // ============================
    if (listen(server_socket, 3) == SOCKET_ERROR) {
        printf("Listen failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server is running on port %d...\n", PORT);
    printf("Waiting for client connection...\n");

    // ============================
    // Step 6: Accept Client
    // ============================
    struct sockaddr_in client_address;
    int client_len = sizeof(client_address);

    SOCKET client_socket = accept(server_socket,
                                  (struct sockaddr *)&client_address,
                                  &client_len);

    if (client_socket == INVALID_SOCKET) {
        printf("Client connection failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Client connected successfully.\n");

    // ============================
    // Step 7: Receive Request
    // ============================
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    recv(client_socket, buffer, BUFFER_SIZE, 0);

    printf("------ Incoming Request ------\n");
    printf("%s\n", buffer);
    printf("------------------------------\n");

    // ============================
    // Step 8: Send HTTP Response
    // ============================
    const char *http_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Hello from FarhanC HTTP Engine v0.2";

    send(client_socket, http_response, strlen(http_response), 0);

    printf("Response sent successfully.\n");

    // ============================
    // Step 9: Cleanup
    // ============================
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    printf("Server shutdown cleanly.\n");

    return 0;
}