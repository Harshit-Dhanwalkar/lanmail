# File Transfer and Chat Application via LAN

This project is a simple file transfer and chat application implemented in C using sockets and threads. The application allows a client to send a text file to a server upon agreement, and also supports simultaneous chat between the client and server.

## Features

- **File Transfer**: The client can send a text file to the server, which is saved in the server's `Received_files` directory.
- **User Agreement**: The server asks for user agreement before accepting the file transfer.
- **Concurrent Chat**: Both client and server can chat with each other independently, using multithreading to handle concurrent send and receive operations.

## Prerequisites

- GCC compiler
- POSIX-compliant operating system (e.g., Linux, macOS)

## Getting Started

### 1. Clone the Repository

```sh
git clone https://github.com/yourusername/your-repo.git
cd your-repo
```
