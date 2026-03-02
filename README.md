# 🐔 Red Hen Kitchen – Custom C HTTP Backend Server

A production-style HTTP backend server built in **pure C using Windows Winsock**.

This project powers the Red Hen Kitchen web application and demonstrates

low-level backend engineering without using any frameworks.

It handles HTTP parsing, routing, static file serving, and request processing

from scratch using raw socket programming.

---

# 🚀 Project Vision

Most developers build backend systems using frameworks.

This project intentionally avoids that.

The goal is to:

- Understand TCP/IP deeply
  
- Implement HTTP manually
  
- Build routing logic from scratch
  
- Serve static assets safely
  
- Design scalable backend architecture
  
- Develop systems-level engineering skills
  

This is backend engineering at the protocol level.

---

# 🏗 Architecture Overview

Browser (Client)

        ↓

TCP Connection (Port 8080)

        ↓

Winsock Socket Layer

        ↓

HTTP Request Buffer Parsing

        ↓

Method + Path Extraction

        ↓

Routing Engine

        ↓

Static File Engine (v0.7)

        ↓

HTTP Response Builder

        ↓

Binary / Text Response to Client

---

# 🛠 Tech Stack

- C (Systems Programming)
  
- Windows Winsock API
  
- TCP/IP
  
- HTTP/1.1 Protocol
  
- Manual Routing Logic
  
- Static File Handling (Binary Mode)


- MIME Type Detection
  
  
No frameworks.

No libraries.

No shortcuts.


---

# 📦 Version History

---

## 🔹 v0.1 – Initial Socket Setup (Pre-release)

The project begins.

Features:

- Winsock initialization
  
- Socket creation
  
- Bind to port 8080
  
- Basic listen setup
  
- Single connection handling
  
- Simple hardcoded HTTP response
  

This version establishes the network foundation.

---

## 🔹 v0.2 – Persistent Server Loop (Pre-release)

Server becomes persistent.

Features:

- Continuous server loop
  
- Sequential client handling
  
- Accept multiple requests
  
- Basic request logging
  

Now the server does not shut down after one request.

---

## 🔹 v0.3 – HTTP Request Parsing (Pre-release)

Server understands client requests.

Features:

- Extract HTTP method (GET)
  
- Extract request path
  
- Basic buffer parsing
  
- Improved console logging


Foundation for routing system created.

---

## 🔹 v0.4 – Route-Based Response Handling (Pre-release)

Routing system introduced.

Features:

- GET `/`
  
- GET `/menu`
  
- Structured HTML responses
  
- 404 fallback support
  
- Organized response builder function
  

Backend transitions from raw socket server to application server.

---

## 🔹 v0.5 – Stable Routing Engine (Official Release)

Routing system stabilized.

Features:

- Continuous server loop
  
- Clean routing logic
  
- HTTP status codes
  
- Proper header construction
  
- Sequential multi-client handling
  
- Structured HTML responses
  

This version marks architectural stability.

---

## 🔹 v0.6 – POST /order Handling

Major functionality upgrade.

Features:
- POST `/order` endpoint
  
- HTTP body extraction
  
- Form data parsing
  
- Order confirmation response
  
- Improved request logging
  

Server now handles dynamic client data.

---

## 🔹 v0.7 – Static File Engine + MIME Detection (Latest Stable)

Backend becomes real-world capable.

Features:
- Serve files from `/public` directory
  
- Default route `/` → `index.html`
  
- Support for:
  
  - HTML
    
  - CSS
    
  - JavaScript
    
  - PNG
    
  - JPG
    
  - ICO
- Binary file reading
  
- MIME type detection
  
- Directory traversal protection (`..` blocked)
  
- Clean scalable file serving logic
  
- Separation of frontend & backend
  

This version transforms the project into a proper static-capable HTTP engine.

---

# 📂 Project Structure

```
FarhanC-HTTP-Engine/

│

├── public/

│   ├── index.html

│   ├── menu.html

│   ├── css/

│   │   └── style.css

│   └── js/

│       └── app.js

│

├── src/

│   └── server.c

│

├── README.md

└── .gitignore

```

---

# 🧪 How To Run

### Compile

```
gcc src/server.c -o server -lws2_32
```

### Run

```
.\server
```

### Open in Browser

```
http://localhost:8080/
http://localhost:8080/menu
```

---

# 🌐 What The Server Currently Supports

✔ Static file serving  

✔ MIME type detection  

✔ Directory traversal protection  

✔ Route handling  

✔ POST request parsing  

✔ Proper HTTP headers  

✔ Content-Length handling  

✔ Sequential multi-client support  

✔ Custom 404 responses  

✔ Clean routing design  

---

# ⚠ Current Limitations

- No concurrency (threading)
  
- No async I/O
  
- No database integration
  
- No persistent storage (yet)
  
- No HTTPS
  
- No caching system

---

# 🎯 Upcoming Roadmap

🔲 v0.8 – File-based order storage  

🔲 v0.9 – Basic admin dashboard  

🔲 v1.0 – Minimal production-ready HTTP backend  

🔲 v1.1 – Multithreaded client handling  

🔲 v1.2 – Logging system + request metrics  

🔲 v2.0 – Linux compatibility + POSIX sockets  


---

# 🧠 Why This Project Matters

Understanding frameworks is good.

Understanding the protocol layer is powerful.

This project builds:

- Systems programming confidence
  
- Networking fundamentals
  
- Manual HTTP understanding
  
- Backend architectural thinking
  

It proves backend knowledge beyond framework usage.

---

# 👨‍💻 Author

Farhan Khan  

Backend Engineering Journey  

Built for learning, scaling, and real-world capability.

---

🔥 Red Hen Kitchen Backend  

From raw sockets to scalable HTTP engine.
