/*
    Project: Red Hen Kitchen Backend Server
    ----------------------------------------
    Version: 0.5

    Description:
    Custom HTTP server built using Windows Winsock.
    This backend powers the Red Hen Kitchen web application.

    Current Capabilities:
    - Continuous server loop
    - Sequential multi-client handling
    - HTTP request parsing (method + path)
    - Route-based response system
    - 404 Not Found handling
    - HTML response rendering

    Supported Routes:
    - GET /
    - GET /menu

    Future Goals:
    - POST /order handling
    - Order storage system
    - Static file serving
    - Database integration
    - Concurrency support

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

        char response[BUFFER_SIZE * 4];
memset(response, 0, sizeof(response));

// ROUTING LOGIC
if (strcmp(path, "/") == 0) {

    sprintf(response,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><head><title>Red Hen Kitchen</title></head>"
        "<h1>Red Hen Kitchen</h1>"
        "<body style='font-family: Arial; text-align:center;'>"
        "<hr>"
        "<p>Authentic homemade meals delivered fresh.</p>"
        "</body></html>"
        "<a href='/menu'>View Our Menu</a>"
    );

}
else if (strcmp(path, "/menu") == 0) {

    sprintf(response,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><head><title>Our Menu - Red Hen Kitchen</title></head>"
        "<h1>Our Menu</h1>"
        "<body style='font-family: Arial; text-align:center;'>"
        "<li>Butter Chicken</li>"
        "<ul style='list-style:none;'>"
        "<li>Chicken Biryani</li>"
        "<li>Paneer Tikka</li>"
        "<hr>"
        "</ul>"
        "</body></html>"
        "<a href='/'>Back to Home</a>"
    );

}
else {

    sprintf(response,
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><body style='text-align:center;'>"
        "<h1>404 - Page Not Found</h1>"
        "<a href='/'>Go Home</a>"
        "</body></html>"
    );

}

send(client_socket, response, strlen(response), 0);

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