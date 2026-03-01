/*
    FarhanC-HTTP-Engine
    ----------------------------------------
    Version: 0.4

    Description:
    A minimal HTTP server built using Windows Winsock.
    This version introduces continuous client handling.
    The server no longer shuts down after serving one request.

    Goal:
    - Handle multiple client connections (sequential)
    - Continuous listening loop

    Learning Focus:
    - Socket lifecycle
    - Persistent server loop
    - Sequential client handling
    - Clean resource management

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

    // -----------------------------------------
    // Step 1: Initialize Winsock Library
    // -----------------------------------------
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Winsock initialization failed.\n");
        return 1;
    }

    // -----------------------------------------
    // Step 2: Create Server Socket
    // -----------------------------------------
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    // -----------------------------------------
    // Step 3: Configure Server Address
    // -----------------------------------------
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // -----------------------------------------
    // Step 4: Bind Socket to Port
    // -----------------------------------------
    if (bind(server_socket,
            (struct sockaddr *)&server_address,
            sizeof(server_address)) == SOCKET_ERROR) {

        printf("Bind failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // -----------------------------------------
    // Step 5: Start Listening for Clients
    // -----------------------------------------
    if (listen(server_socket, 5) == SOCKET_ERROR) {
        printf("Listen failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server is running on port %d...\n", PORT);
    printf("Waiting for incoming connections...\n");

    // =========================================
    // Main Server Loop (v0.3 Feature)
    // Server stays alive indefinitely
    // =========================================
    while (1) {

        struct sockaddr_in client_address;
        int client_len = sizeof(client_address);

        // -------------------------------------
        // Accept incoming client connection
        // -------------------------------------
        SOCKET client_socket = accept(server_socket,
                                      (struct sockaddr *)&client_address,
                                      &client_len);

        if (client_socket == INVALID_SOCKET) {
            printf("Client connection failed. Continuing...\n");
            continue;  // Do NOT shut down server
        }

        printf("Client connected successfully.\n");

        // -------------------------------------
        // Receive HTTP request
        // -------------------------------------
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);

        recv(client_socket, buffer, BUFFER_SIZE, 0);
        // Extract request method and path
        char path[100];
        char method[10];
        sscanf(buffer, "%s %s", method, path);

        // Simple request logging
        printf("[LOG] Method: %s | Path: %s\n", method, path);

        printf("------ Incoming Request ------\n");
        printf("%s\n", buffer);
        printf("------------------------------\n");

        // -------------------------------------
        // Send HTTP response
        // -------------------------------------
        const char *http_response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "Hello from FarhanC HTTP Engine v0.4";

        send(client_socket, http_response, strlen(http_response), 0);

        printf("Response sent successfully.\n");

        // -------------------------------------
        // Close only client socket
        // Server remains active
        // -------------------------------------
        closesocket(client_socket);
    }

    // Normally unreachable unless program manually stopped
    closesocket(server_socket);
    WSACleanup();

    return 0;
}