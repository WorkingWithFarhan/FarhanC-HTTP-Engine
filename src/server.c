/*
===========================================================
FarhanC HTTP Engine
Version: V0.9

Simple HTTP server written in C.

Evolution of versions:

V0.1  Basic socket server
V0.2  Receive HTTP request
V0.3  Parse HTTP method and path
V0.4  Serve index.html
V0.5  Routing system
V0.6  Serve CSS static file
V0.7  Admin page route
V0.8  API route example
V0.9  Health check, metrics, server info, request logging

Author: Farhan
===========================================================
*/

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 4096


/* ==============================
   Global server metrics
   ============================== */

int total_requests = 0;
time_t server_start_time;


/* ==============================
   Send HTTP response helper
   ============================== */

void send_response(SOCKET client, const char *type, const char *body)
{
    char response[BUFFER_SIZE];

    sprintf(response,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s",
        type,
        (int)strlen(body),
        body
    );

    send(client, response, strlen(response), 0);
}


/* ==============================
   Serve static files (HTML/CSS)
   ============================== */

void serve_file(SOCKET client, const char *filename, const char *type)
{
    FILE *file = fopen(filename, "rb");

    if (!file)
    {
        const char *not_found =
        "<html><body><h1>404 - Page Not Found</h1></body></html>";

        send_response(client, "text/html", not_found);
        return;
    }

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    char *buffer = malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);

    send_response(client, type, buffer);

    free(buffer);
}


/* ==============================
   Get server uptime
   ============================== */

long get_uptime_seconds()
{
    time_t now = time(NULL);
    return now - server_start_time;
}


/* ==============================
   ROUTER
   ============================== */

void route_request(SOCKET client, const char *path)
{

    /* Landing page */
    if (strcmp(path, "/") == 0)
    {
        serve_file(client, "public/index.html", "text/html");
        return;
    }


    /* Menu page */
    if (strcmp(path, "/menu") == 0)
    {
        serve_file(client, "public/menu.html", "text/html");
        return;
    }


    /* Admin dashboard */
    if (strcmp(path, "/admin") == 0)
    {
        serve_file(client, "public/admin.html", "text/html");
        return;
    }


    /* CSS file */
    if (strcmp(path, "/css/style.css") == 0)
    {
        serve_file(client, "css/style.css", "text/css");
        return;
    }


    /* ======================
       V0.9 HEALTH ENDPOINT
       ====================== */

    if (strcmp(path, "/health") == 0)
    {
        send_response(client, "text/plain", "OK");
        return;
    }


    /* ======================
       V0.9 INFO ENDPOINT
       ====================== */

    if (strcmp(path, "/info") == 0)
    {
        const char *info =
        "FarhanC HTTP Engine\n"
        "Version: 0.9\n"
        "Language: C\n"
        "Author: Farhan\n";

        send_response(client, "text/plain", info);
        return;
    }


    /* ======================
       V0.9 METRICS ENDPOINT
       ====================== */

    if (strcmp(path, "/metrics") == 0)
    {
        char metrics[512];

        sprintf(metrics,
        "{\n"
        "  \"total_requests\": %d,\n"
        "  \"uptime_seconds\": %ld,\n"
        "  \"version\": \"0.9\"\n"
        "}",
        total_requests,
        get_uptime_seconds());

        send_response(client, "application/json", metrics);
        return;
    }


    /* Example API */
    if (strcmp(path, "/api/orders") == 0)
    {
        const char *json =
        "{ \"status\": \"API working\", \"version\": \"0.8\" }";

        send_response(client, "application/json", json);
        return;
    }


    /* Default 404 */

    const char *not_found =
    "<html><body><h1>404 - Page Not Found</h1></body></html>";

    send_response(client, "text/html", not_found);
}



/* ==============================
   MAIN SERVER
   ============================== */

int main()
{
    WSADATA wsa;
    SOCKET server_socket, client_socket;

    struct sockaddr_in server, client;
    int c;

    char buffer[BUFFER_SIZE];

    WSAStartup(MAKEWORD(2,2), &wsa);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr *)&server, sizeof(server));

    listen(server_socket, 3);

    printf("Server running at http://localhost:%d\n", PORT);

    server_start_time = time(NULL);

    c = sizeof(struct sockaddr_in);


    while (1)
    {
        client_socket = accept(server_socket,
        (struct sockaddr *)&client, &c);

        int recv_size = recv(client_socket, buffer,
        BUFFER_SIZE - 1, 0);

        if (recv_size <= 0)
        {
            closesocket(client_socket);
            continue;
        }

        buffer[recv_size] = '\0';

        char method[16];
        char path[256];

        sscanf(buffer, "%s %s", method, path);

        total_requests++;


        /* ===== Request logging ===== */

        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        printf("[%02d:%02d:%02d] %s %s\n",
        t->tm_hour,
        t->tm_min,
        t->tm_sec,
        method,
        path);


        route_request(client_socket, path);

        closesocket(client_socket);
    }

    closesocket(server_socket);
    WSACleanup();

    return 0;
}