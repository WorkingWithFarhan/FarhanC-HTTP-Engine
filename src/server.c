#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 8192

const char* get_mime_type(const char* path) {

    const char* ext = strrchr(path, '.');
    if (!ext) return "text/plain";

    if (strcmp(ext, ".html") == 0) return "text/html";
    if (strcmp(ext, ".css") == 0) return "text/css";
    if (strcmp(ext, ".js") == 0) return "application/javascript";
    return "application/octet-stream";
}

void send_404(SOCKET client_socket) {

    const char* response =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<h1>404 - File Not Found</h1>";

    send(client_socket, response, strlen(response), 0);
}

void serve_file(SOCKET client_socket, const char* path) {

    char full_path[512];

    if (strcmp(path, "/") == 0)
        sprintf(full_path, "public/index.html");
    else if (strcmp(path, "/menu") == 0)
        sprintf(full_path, "public/menu.html");
    else
        sprintf(full_path, "public%s", path);

    FILE* file = fopen(full_path, "rb");
    if (!file) {
        send_404(client_socket);
        return;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* buffer = malloc(size);
    fread(buffer, 1, size, file);
    fclose(file);

    char header[256];
    sprintf(header,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n\r\n",
        get_mime_type(full_path), size);

    send(client_socket, header, strlen(header), 0);
    send(client_socket, buffer, size, 0);

    free(buffer);
}

void handle_order(SOCKET client_socket, const char* buffer) {

    char* body = strstr(buffer, "\r\n\r\n");
    if (body) body += 4;

    printf("\n=== NEW ORDER ===\n");
    printf("%s\n", body);
    printf("=================\n");

    const char* response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n\r\n"
        "{\"message\":\"Order received successfully\"}";

    send(client_socket, response, strlen(response), 0);
}

int main() {

    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    int client_len = sizeof(client_address);

    WSAStartup(MAKEWORD(2,2), &wsa);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    bind(server_socket,
         (struct sockaddr*)&server_address,
         sizeof(server_address));

    listen(server_socket, 5);

    printf("Server running → http://localhost:%d\n", PORT);

    while (1) {

        client_socket = accept(server_socket,
                               (struct sockaddr*)&client_address,
                               &client_len);

        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        recv(client_socket, buffer, BUFFER_SIZE, 0);

        char method[10], path[256];
        sscanf(buffer, "%s %s", method, path);

        printf("[REQUEST] %s %s\n", method, path);

        if (strcmp(method, "POST") == 0 && strcmp(path, "/order") == 0) {
            handle_order(client_socket, buffer);
            closesocket(client_socket);
            continue;
        }

        if (strcmp(method, "GET") == 0) {
            serve_file(client_socket, path);
            closesocket(client_socket);
            continue;
        }

        send_404(client_socket);
        closesocket(client_socket);
    }

    return 0;
}