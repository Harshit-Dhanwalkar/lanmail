// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Invalid data\n");
        exit(EXIT_FAILURE);
    }
    
    // Create a socket
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Giving address for socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[2]));
    if(strcmp(argv[1], "loopback") == 0){
        server_address.sin_addr.s_addr = INADDR_ANY;
    }
    else{
        server_address.sin_addr.s_addr = inet_addr(argv[1]);
    }
    
    // Connecting to server
    int connection_status = connect(network_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    if(connection_status == -1){
        printf("Error in connection\n");
        exit(1);
    }

    char data[256];
    recv(network_socket, data, sizeof(data), 0);
    printf("%s\n", data);


    // Chatting begins
    while(1){
        char data[256];
        printf("Client : ");
        fgets(data, 256, stdin);
        send(network_socket, data, sizeof(data), 0);
        printf("\n");
        recv(network_socket, data, sizeof(data), 0);
        printf("Response : %s\n", data);
    }
    close(network_socket);
    return 0;
}
