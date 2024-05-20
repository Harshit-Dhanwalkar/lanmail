// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024
#define RECEIVED_FILES_DIR "Received_files"

void write_file(int sockfd) {
    char buffer[BUFFER_SIZE];
    FILE *fp = fopen(RECEIVED_FILES_DIR "/received_file.txt", "w");
    if (fp == NULL) {
        perror("Error in opening file");
        exit(EXIT_FAILURE);
    }
    while (1) {
        ssize_t n = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (n <= 0 || strcmp(buffer, "FILE_TRANSFER_COMPLETE") == 0) {
            break;
        }
        fprintf(fp, "%s", buffer);
        memset(buffer, 0, BUFFER_SIZE);
    }
    fclose(fp);
}

void chat(int sockfd) {
    char buffer[BUFFER_SIZE];
    while (1) {
        printf("\n");
        memset(buffer, 0, BUFFER_SIZE);
        recv(sockfd, buffer, BUFFER_SIZE, 0);
        printf("Client: %s", buffer);
        memset(buffer, 0, BUFFER_SIZE);
        printf("\n")
        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(sockfd, buffer, strlen(buffer), 0);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Create the "Received_files" directory if it doesn't exist
    struct stat st = {0};
    if (stat(RECEIVED_FILES_DIR, &st) == -1) {
        mkdir(RECEIVED_FILES_DIR, 0700);
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[1]));
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) == -1) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
        perror("Client connection failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Ask for user agreement
    printf("Do you want to receive the file? (y/n): ");
    char agreement;
    scanf(" %c", &agreement);

    if (send(client_socket, &agreement, sizeof(agreement), 0) == -1) {
        perror("Error in sending agreement to client");
        close(client_socket);
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (agreement == 'y') {
        write_file(client_socket);
        printf("File received successfully.\n");
    } else {
        printf("File transfer rejected.\n");
    }

    // Continue to chat regardless of file transfer outcome
    chat(client_socket);

    close(client_socket);
    close(server_socket);

    return 0;
}
