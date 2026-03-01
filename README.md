# 🐔 Red Hen Kitchen – Backend Server

A custom HTTP backend server built in C using Windows Winsock.

This project powers the Red Hen Kitchen website.

It handles HTTP requests, routes different paths, and serves dynamic responses.

---

## 🚀 Project Purpose

Red Hen Kitchen is a food ordering web application.

This backend server is responsible for:

- Handling client connections
- 
- Processing HTTP requests
- 
- Routing different URLs
- 
- Serving HTML responses
- 
- Returning proper HTTP status codes
- 
- Managing multiple client requests sequentially
- 

This project focuses on building backend fundamentals from scratch
without using any external frameworks.

---

## 🏗 Architecture Overview

Browser (Client)

        ↓
        
TCP Connection (Port 8080)

        ↓
        
Custom Winsock HTTP Server

        ↓
        
Request Parsing (Method + Path)

        ↓
        
Routing Engine

        ↓
        
HTML Response / 404

---

## 📌 Current Version

### ✅ v0.5 – Basic Routing System

Features:

- Continuous server loop
- 
- Multiple client handling (sequential)
- 
- HTTP request parsing
- 
- Route-based response handling
- 
- 404 Not Found support
- 
- Simple HTML rendering
- 
- Console logging

---

## 🛠 Tech Stack

- C (Systems Programming)
- 
- Windows Winsock API
- 
- TCP/IP
- 
- HTTP/1.1 Protocol
- 

---

## 🧪 How to Run

Compile:


gcc src/server.c -o server -lws2_32


Run:


.\server


Open browser:


http://localhost:8080/

http://localhost:8080/menu

---

## 📂 Project Structure


Red-Hen-Kitchen/

│

├── src/

│ └── server.c

│

├── README.md

└── .gitignore


---

## 🧠 What This Server Currently Supports

✔ Home route (`/`)  

✔ Menu route (`/menu`)  

✔ 404 handling  

✔ Sequential multi-client support  

✔ Manual routing logic  

✔ Clean HTTP response structure  


---

## ❌ Current Limitations

- No POST handling yet
- 
- No database integration
- 
- No static file serving
- 
- No CSS/JS external files
- 
- No concurrency (threading)
- 
- No HTTPS

---

## 🎯 Next Milestones

🔲 v0.6 – Handle POST /order  

🔲 v0.7 – Serve external HTML files  

🔲 v0.8 – File-based order storage  

🔲 v0.9 – Basic admin dashboard

🔲 v1.0 – Production-ready minimal food ordering backend  

---

## 📖 Why Build This?

Most developers use frameworks.

This project builds the backend foundation from scratch.

Understanding sockets + HTTP deeply creates strong backend engineers.
