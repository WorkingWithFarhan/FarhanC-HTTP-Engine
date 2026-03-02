/*
------------------------------------------------------------
Red Hen Kitchen - Backend Server
------------------------------------------------------------
Version: 0.6

A minimal HTTP backend written in C using Windows Winsock.

Purpose:
This server powers the Red Hen Kitchen demo website.
It supports basic routing and form submission handling
without any external frameworks.

Features:
- Persistent server loop
- Sequential client handling
- HTTP request parsing (method + path)
- GET routing
- POST /order form handling
- Basic request body extraction
- Clean response builder
- 404 fallback

Author: Farhan Khan
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 4096

/* ---------------------------------------------------------
   Helper: Send HTTP Response
--------------------------------------------------------- */
void send_response(SOCKET client_socket,
                   const char *status,
                   const char *content_type,
                   const char *body)
{
    char header[BUFFER_SIZE];

    int body_length = (int)strlen(body);

    sprintf(header,
        "HTTP/1.1 %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n",
        status,
        content_type,
        body_length
    );

    send(client_socket, header, strlen(header), 0);
    send(client_socket, body, body_length, 0);
}

/* ---------------------------------------------------------
   Entry Point
--------------------------------------------------------- */
int main()
{
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    int client_len = sizeof(client_address);

    /* ---------------- Initialize Winsock ---------------- */
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("[ERROR] Winsock initialization failed.\n");
        return 1;
    }

    /* ---------------- Create Socket ---------------- */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("[ERROR] Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    /* ---------------- Configure Address ---------------- */
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    /* ---------------- Bind ---------------- */
    if (bind(server_socket,
            (struct sockaddr*)&server_address,
            sizeof(server_address)) == SOCKET_ERROR) {

        printf("[ERROR] Bind failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    /* ---------------- Listen ---------------- */
    if (listen(server_socket, 5) == SOCKET_ERROR) {
        printf("[ERROR] Listen failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("--------------------------------------------------\n");
    printf(" Red Hen Kitchen Backend Running\n");
    printf(" http://localhost:%d\n", PORT);
    printf("--------------------------------------------------\n");

    /* =====================================================
       Main Server Loop
    ===================================================== */
    while (1) {

        client_socket = accept(server_socket,
                               (struct sockaddr*)&client_address,
                               &client_len);

        if (client_socket == INVALID_SOCKET) {
            printf("[ERROR] Failed to accept client.\n");
            continue;
        }

        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);

        recv(client_socket, buffer, BUFFER_SIZE, 0);

        char method[16];
        char path[256];

        sscanf(buffer, "%s %s", method, path);

        printf("[REQUEST] %s %s\n", method, path);

        /* =================================================
           ROUTING SECTION
        ================================================= */

        /* ---------- GET / ---------- */
        if (strcmp(method, "GET") == 0 &&
            strcmp(path, "/") == 0)
        {
            const char *home_page =
                "<html>"
                "<head><title>Red Hen Kitchen</title></head>"
                "<body style='font-family:Arial;text-align:center;'>"
                "<h1>🔥 Red Hen Kitchen 🔥</h1>"
                "<p>Authentic homemade meals delivered fresh.</p>"
                "<a href='/menu'>View Menu</a>"
                "<hr>"
                "<h3>Place an Order</h3>"
                "<form method='POST' action='/order'>"
                "Name:<br><input name='name'><br><br>"
                "Item:<br><input name='item'><br><br>"
                "Address:<br><input name='address'><br><br>"
                "<button type='submit'>Order Now</button>"
                "</form>"
                "</body>"
                "</html>";

            send_response(client_socket, "200 OK", "text/html", home_page);
        }

        /* ---------- GET /menu ---------- */
        else if (strcmp(method, "GET") == 0 &&
                 strcmp(path, "/menu") == 0)
        {
            const char *menu_page =
                "<html>"
                "<head><title>Menu - Red Hen Kitchen</title></head>"
                "<body style='font-family:Arial;text-align:center;'>"
                "<h1>Our Menu</h1>"
                "<ul style='list-style:none;'>"
                "<li>Butter Chicken</li>"
                "<li>Chicken Biryani</li>"
                "<li>Paneer Tikka</li>"
                "</ul>"
                "<a href='/'>Back Home</a>"
                "</body>"
                "</html>";

            send_response(client_socket, "200 OK", "text/html", menu_page);
        }

        /* ---------- POST /order ---------- */
        else if (strcmp(method, "POST") == 0 &&
                 strcmp(path, "/order") == 0)
        {
            /* Extract request body */
            char *body = strstr(buffer, "\r\n\r\n");
            if (body != NULL) {
                body += 4;  // Skip header separator
            }

            printf("--------------------------------------------------\n");
            printf("[ORDER RECEIVED]\n");
            printf("Raw Form Data: %s\n", body ? body : "No body found");
            printf("--------------------------------------------------\n");

            const char *confirmation_page =
                "<html>"
                "<head><title>Order Confirmed</title></head>"
                "<body style='font-family:Arial;text-align:center;'>"
                "<h1>✅ Order Placed Successfully!</h1>"
                "<p>Thank you for choosing Red Hen Kitchen.</p>"
                "<a href='/'>Back to Home</a>"
                "</body>"
                "</html>";

            send_response(client_socket, "200 OK", "text/html", confirmation_page);
        }

        /* ---------- 404 ---------- */
        else
        {
            const char *not_found =
                "<html>"
                "<head><title>404</title></head>"
                "<body style='font-family:Arial;text-align:center;'>"
                "<h1>404 - Page Not Found</h1>"
                "<a href='/'>Return Home</a>"
                "</body>"
                "</html>";

            send_response(client_socket, "404 Not Found", "text/html", not_found);
        }

        closesocket(client_socket);
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}