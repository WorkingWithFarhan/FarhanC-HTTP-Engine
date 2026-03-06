/*
===============================================================
FarhanC HTTP Engine
Version: V1.0

Author: Farhan

---------------------------------------------------------------
Project Description
---------------------------------------------------------------

FarhanC HTTP Engine is a lightweight HTTP server written in C.
The goal of this project is to understand how web servers work
internally while building a practical backend system.

This server powers a simple restaurant website called:

    "Red Hen Kitchen"

The server handles:

• HTTP requests
• Static file serving
• Route based navigation
• JSON API endpoints
• Server monitoring endpoints
• Basic server observability
• Dynamic HTML generation

---------------------------------------------------------------
Version History
---------------------------------------------------------------

V0.1  Basic socket server
V0.2  Receive HTTP request
V0.3  Parse HTTP method and path
V0.4  Serve index.html
V0.5  Routing system
V0.6  Serve CSS static file
V0.7  Admin page route
V0.8  API route example
V0.9  Health, metrics, server info, logging
V1.0  Server dashboard endpoint

---------------------------------------------------------------
Architecture Overview
---------------------------------------------------------------

Client Browser
      │
      ▼
HTTP Request (GET /menu)
      │
      ▼
Socket Server (Winsock)
      │
      ▼
Request Parser
      │
      ▼
Router
 ├─ Static file
 ├─ API endpoint
 ├─ Metrics
 └─ Dashboard
      │
      ▼
HTTP Response

===============================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 4096


/*
===============================================================
GLOBAL SERVER METRICS
===============================================================

These variables allow us to track runtime statistics
about the server.

These metrics are used by:

/metrics
/dashboard
/stats

*/

long total_requests = 0;
time_t server_start_time;



/*
===============================================================
UTILITY FUNCTION
Calculate server uptime
===============================================================
*/

long get_uptime_seconds()
{
    time_t now = time(NULL);
    return now - server_start_time;
}



/*
===============================================================
HTTP RESPONSE HELPER
===============================================================

Constructs a valid HTTP response and sends it
back to the client.

This helper keeps the code cleaner since all
routes can reuse this function.

*/

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



/*
===============================================================
STATIC FILE SERVER
===============================================================

Reads a file from disk and sends it as HTTP response.

Used for:

/index.html
/menu.html
/admin.html
/css/style.css

*/

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



/*
===============================================================
V1.0 DASHBOARD ENDPOINT
===============================================================

This endpoint dynamically generates an HTML page
showing real time server statistics.

URL:

    http://localhost:8080/dashboard

Displayed Information:

• Server status
• Total requests handled
• Server uptime

*/

void send_dashboard(SOCKET client)
{
    char html[BUFFER_SIZE];

    long uptime = get_uptime_seconds();

    sprintf(html,

    "<html>"
    "<head>"
    "<title>Server Dashboard</title>"

    "<style>"
    "body{font-family:Arial;background:#0f172a;color:white;text-align:center;padding:40px;}"
    ".card{background:#1e293b;padding:30px;border-radius:10px;width:420px;margin:auto;}"
    "h1{color:#38bdf8;}"
    "</style>"

    "</head>"

    "<body>"

    "<h1>FarhanC HTTP Engine Dashboard</h1>"

    "<div class='card'>"

    "<p><b>Status:</b> Running</p>"
    "<p><b>Total Requests:</b> %ld</p>"
    "<p><b>Server Uptime:</b> %ld seconds</p>"

    "</div>"

    "</body>"
    "</html>",

    total_requests,
    uptime
    );

    send_response(client, "text/html", html);
}



/*
===============================================================
ROUTING SYSTEM
===============================================================

The router determines which code should handle
each incoming request path.

Example:

GET /menu
GET /metrics
GET /dashboard

*/

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


    /* Admin panel */

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



    /*
    ===========================================================
    V1.0 DASHBOARD
    ===========================================================
    */

    if (strcmp(path, "/dashboard") == 0)
    {
        send_dashboard(client);
        return;
    }



    /*
    ===========================================================
    HEALTH ENDPOINT
    ===========================================================
    */

    if (strcmp(path, "/health") == 0)
    {
        send_response(client, "text/plain", "OK");
        return;
    }



    /*
    ===========================================================
    SERVER INFO
    ===========================================================
    */

    if (strcmp(path, "/info") == 0)
    {
        const char *info =
        "FarhanC HTTP Engine\n"
        "Version: 1.0\n"
        "Language: C\n"
        "Author: Farhan\n";

        send_response(client, "text/plain", info);
        return;
    }



    /*
    ===========================================================
    METRICS ENDPOINT
    ===========================================================
    */

    if (strcmp(path, "/metrics") == 0)
    {
        char metrics[512];

        sprintf(metrics,
        "{\n"
        "  \"total_requests\": %ld,\n"
        "  \"uptime_seconds\": %ld,\n"
        "  \"version\": \"1.0\"\n"
        "}",
        total_requests,
        get_uptime_seconds());

        send_response(client, "application/json", metrics);
        return;
    }



    /*
    ===========================================================
    API ENDPOINT EXAMPLE
    ===========================================================
    */

    if (strcmp(path, "/api/orders") == 0)
    {
        const char *json =
        "{ \"status\": \"API working\", \"version\": \"0.8\" }";

        send_response(client, "application/json", json);
        return;
    }



    /*
    ===========================================================
    DEFAULT 404 RESPONSE
    ===========================================================
    */

    const char *not_found =
    "<html><body><h1>404 - Page Not Found</h1></body></html>";

    send_response(client, "text/html", not_found);
}




/*
===============================================================
MAIN SERVER
===============================================================

Responsibilities:

• Initialize Winsock
• Create TCP socket
• Bind server port
• Listen for connections
• Accept clients
• Parse HTTP request
• Route request
• Send response

*/

int main()
{
    WSADATA wsa;
    SOCKET server_socket, client_socket;

    struct sockaddr_in server, client;
    int c;

    char buffer[BUFFER_SIZE];



    /*
    -----------------------------------------------------------
    Initialize Winsock
    -----------------------------------------------------------
    */

    WSAStartup(MAKEWORD(2,2), &wsa);



    /*
    -----------------------------------------------------------
    Create server socket
    -----------------------------------------------------------
    */

    server_socket = socket(AF_INET, SOCK_STREAM, 0);



    /*
    -----------------------------------------------------------
    Configure server address
    -----------------------------------------------------------
    */

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);



    /*
    -----------------------------------------------------------
    Bind socket to port
    -----------------------------------------------------------
    */

    bind(server_socket, (struct sockaddr *)&server, sizeof(server));



    /*
    -----------------------------------------------------------
    Start listening
    -----------------------------------------------------------
    */

    listen(server_socket, 3);



    printf("\n=====================================\n");
    printf("FarhanC HTTP Engine v1.0\n");
    printf("Server running at http://localhost:%d\n", PORT);
    printf("=====================================\n\n");



    server_start_time = time(NULL);

    c = sizeof(struct sockaddr_in);



    /*
    ===========================================================
    MAIN SERVER LOOP
    ===========================================================
    */

    while (1)
    {

        client_socket = accept(
            server_socket,
            (struct sockaddr *)&client,
            &c
        );



        int recv_size = recv(
            client_socket,
            buffer,
            BUFFER_SIZE - 1,
            0
        );

        if (recv_size <= 0)
        {
            closesocket(client_socket);
            continue;
        }

        buffer[recv_size] = '\0';



        /*
        -------------------------------------------------------
        Parse HTTP request
        -------------------------------------------------------
        */

        char method[16];
        char path[256];

        sscanf(buffer, "%s %s", method, path);



        total_requests++;



        /*
        -------------------------------------------------------
        Request logging
        -------------------------------------------------------
        */

        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        printf("[INFO] %02d:%02d:%02d | %s %s\n",
            t->tm_hour,
            t->tm_min,
            t->tm_sec,
            method,
            path
        );



        /*
        -------------------------------------------------------
        Route request
        -------------------------------------------------------
        */

        route_request(client_socket, path);



        closesocket(client_socket);
    }



    closesocket(server_socket);
    WSACleanup();

    return 0;
}