// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void send_file(FILE *fp, int sockfd) {
    char buffer[BUFFER_SIZE] = {0};
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        if (send(sockfd, buffer, strlen(buffer), 0) == -1) {
            perror("Error in sending file.");
            exit(EXIT_FAILURE);
        }
        memset(buffer, 0, BUFFER_SIZE);
    }
    // Notify server that file transfer is complete
    strcpy(buffer, "FILE_TRANSFER_COMPLETE");
    send(sockfd, buffer, strlen(buffer), 0);
}

void chat(int sockfd) {
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(sockfd, buffer, strlen(buffer), 0);
        memset(buffer, 0, BUFFER_SIZE);
        recv(sockfd, buffer, BUFFER_SIZE, 0);
        printf("Server: %s", buffer);
        printf("\n");  
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <server_ip> <server_port> <file_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int network_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (network_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[2]));

    if (strcmp(argv[1], "loopback") == 0) {
        server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    } else {
        server_address.sin_addr.s_addr = inet_addr(argv[1]);
    }

    if (connect(network_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Connection failed");
        close(network_socket);
        exit(EXIT_FAILURE);
    }

    // Check server response for agreement
    char server_response;
    if (recv(network_socket, &server_response, sizeof(server_response), 0) <= 0) {
        perror("Error in receiving response from server");
        close(network_socket);
        exit(EXIT_FAILURE);
    }

    if (server_response == 'y') {
        FILE *fp = fopen(argv[3], "r");
        if (fp == NULL) {
            perror("File not found");
            close(network_socket);
            exit(EXIT_FAILURE);
        }

        send_file(fp, network_socket);
        printf("File data sent successfully.\n");
        fclose(fp);
    } else {
        printf("Server did not accept the file transfer.\n");
    }

    chat(network_socket);

    close(network_socket);
    return 0;
}
