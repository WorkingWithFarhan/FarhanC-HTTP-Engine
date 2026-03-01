🔥 FarhanC-HTTP-Engine 🔥

A minimal HTTP server built from scratch in C using Windows Winsock.

Built to deeply understand:

Socket programming
HTTP protocol internals
Client-server communication
Low-level networking fundamentals

🚀 Project Vision 🚀

This project is not just a “server”.
It is a learning-driven systems engineering journey.
The goal is to:

1) Understand how real web servers work internally
2) Build networking fundamentals from scratch
3) Move toward production-grade backend systems

🏗 Architecture Overview 🏗

Client (Browser / curl)

↓

TCP Connection (Port 8080)

↓

Winsock Socket Server

↓

HTTP Request Parsing

↓

HTTP Response Generation


📌 Current Versions 📌

✅ v0.1 – Basic Socket Setup
Winsock initialization
Socket creation
Bind to port
Basic connection handling

✅ v0.2 – Single Request Handling
Accept single client
Receive HTTP request
Send static HTTP response
Clean shutdown

✅ v0.3 – Multi-Client Support (Sequential)
Infinite loop with accept()
Handles multiple clients one-by-one
Server runs continuously

✅ v0.4 – Structured Logging
Clean log formatting
[INFO], [REQUEST], [RESPONSE], [ERROR] prefixes
Better console readability
Debug-friendly output

🛠 Technologies Used 🛠

C (Systems Programming)
Windows Winsock API

TCP/IP

HTTP/1.1 Protocol


🧪 How to Run 🧪

1) Compile:
gcc src/server.c -o server -lws2_32


2) Run:
.\server


3) Open browser:
http://localhost:8080


📂 Project Structure 📂

FarhanC-HTTP-Engine/
│

├── src/

│ ├── server.c

│ └── .gitignore

│

├── README.md

└── .gitignore


🧠 What This Server Currently Does 🧠

✔ Accepts HTTP connections

✔ Prints full request to console

✔ Sends valid HTTP 200 response

✔ Handles multiple clients sequentially

✔ Structured console logging


❌ Current Limitations ❌

Not concurrent (no threads yet)

No static file serving

No routing

No HTTPS

No persistent connections

No file-based logging


🎯 Roadmap 🎯

🔲 v0.5 – File-based logging

🔲 v0.6 – Static HTML file serving

🔲 v0.7 – Basic routing

🔲 v0.8 – Thread-based concurrency

🔲 v1.0 – Production-ready minimal HTTP engine


📖 Why This Project Matters 📖

Most developers use frameworks

Few understand what happens beneath them

This project builds that foundation

Understanding sockets + HTTP at low level makes backend engineering stronger
