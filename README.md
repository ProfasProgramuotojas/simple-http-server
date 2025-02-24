# Simple HTTP Server in C++

This is a basic HTTP server implemented in C++ using Winsock2, which listens for incoming HTTP requests and responds with a simple HTML page based on the route in the request URL.

## Requirements

- Windows operating system (as it uses Winsock2 for networking).
- A C++ compiler (e.g., `g++`).

## Compilation

To compile the code, go to this projects location and use the following command:

```bash
g++ main.cpp -o main.exe -lws2_32
```
## Running the server

To run the server, use the following command:

```bash
./main.exe
