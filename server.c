#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Invalid input\n");
        exit(EXIT_FAILURE);
    }
    // Creating socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Define server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[1]));
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket to the specified IP and port
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    listen(server_socket, 5);
    // creating client socket
    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);


    char message[] = "You are connected to the server";
    send(client_socket, message, sizeof(message), 0);

    while(1){
        char data[256];
        recv(client_socket, data, sizeof(data), 0);
        printf("Response : %s\n", data);
        printf("Server : ");
        fgets(data, 256, stdin);
        send(client_socket, data, sizeof(data), 0);
        printf("\n");
    }

    close(server_socket);
}
