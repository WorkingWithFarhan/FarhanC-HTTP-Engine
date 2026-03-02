/*
---------------------------------------------------------
Red Hen Kitchen - Backend Server
Version: 0.7
Static File Engine (Clean Architecture)
---------------------------------------------------------

This server:

- Serves static files from /public
- Detects MIME types
- Prevents directory traversal
- Supports HTML, CSS, JS, PNG, JPG, ICO
- Continuous request loop
- Clean structured logging

Author: Farhan Khan
---------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 8192

// ---------------------------------------------------------
// MIME TYPE DETECTION
// ---------------------------------------------------------
const char* get_mime_type(const char* path) {

    const char* ext = strrchr(path, '.');

    if (!ext) return "text/plain";

    if (strcmp(ext, ".html") == 0) return "text/html";
    if (strcmp(ext, ".css") == 0) return "text/css";
    if (strcmp(ext, ".js") == 0) return "application/javascript";
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".jpg") == 0) return "image/jpeg";
    if (strcmp(ext, ".ico") == 0) return "image/x-icon";

    return "application/octet-stream";
}

// ---------------------------------------------------------
// PATH SAFETY CHECK
// ---------------------------------------------------------
int is_path_safe(const char* path) {
    if (strstr(path, "..")) return 0;
    return 1;
}

// ---------------------------------------------------------
// SEND 404 RESPONSE
// ---------------------------------------------------------
void send_404(SOCKET client_socket) {

    const char* response =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<h1 style='text-align:center;'>404 - File Not Found</h1>";

    send(client_socket, response, strlen(response), 0);
}

// ---------------------------------------------------------
// SERVE STATIC FILE
// ---------------------------------------------------------
void serve_file(SOCKET client_socket, const char* request_path) {

    char full_path[512];

    // Default route
    if (strcmp(request_path, "/") == 0) {
        sprintf(full_path, "public/index.html");
    } else {
        sprintf(full_path, "public%s", request_path);
    }

    FILE* file = fopen(full_path, "rb");

    if (!file) {
        printf("[ERROR] File not found: %s\n", full_path);
        send_404(client_socket);
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char* buffer = malloc(file_size);
    fread(buffer, 1, file_size, file);
    fclose(file);

    const char* mime = get_mime_type(full_path);

    char header[512];
    sprintf(header,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n\r\n",
        mime,
        file_size
    );

    send(client_socket, header, strlen(header), 0);
    send(client_socket, buffer, file_size, 0);

    free(buffer);
}

// ---------------------------------------------------------
// MAIN SERVER LOOP
// ---------------------------------------------------------
int main() {

    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    int client_len = sizeof(client_address);

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("[FATAL] Winsock init failed.\n");
        return 1;
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    bind(server_socket,
         (struct sockaddr*)&server_address,
         sizeof(server_address));

    listen(server_socket, 5);

    printf("Red Hen Kitchen Backend Running → http://localhost:%d\n", PORT);

    while (1) {

        client_socket = accept(server_socket,
                               (struct sockaddr*)&client_address,
                               &client_len);

        char request_buffer[BUFFER_SIZE];
        memset(request_buffer, 0, BUFFER_SIZE);

        recv(client_socket, request_buffer, BUFFER_SIZE, 0);

        char method[10];
        char path[256];

        sscanf(request_buffer, "%s %s", method, path);

        printf("[REQUEST] %s %s\n", method, path);

        // Only GET allowed in static engine
        if (strcmp(method, "GET") != 0) {
            send_404(client_socket);
            closesocket(client_socket);
            continue;
        }

        // Path security
        if (!is_path_safe(path)) {
            printf("[SECURITY] Blocked unsafe path\n");
            send_404(client_socket);
            closesocket(client_socket);
            continue;
        }

        serve_file(client_socket, path);

        closesocket(client_socket);
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}