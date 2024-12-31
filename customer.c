#include "restaurant.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <name> <item1> <item2> ...\n", argv[0]);
        exit(-1);
    }

    int clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[ITEMS_START + MAX_ITEMS];
    int status, totalItems = argc - 2;
    printf("SERVER: Received client connection.\n");
    printf("CLIENT: Sending order from %s to server.\n", argv[1]);

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket < 0) {
        perror("Socket creation failed");
        exit(-1);
    }

    // Setup server address
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(RESTAURANT_IP);
    serverAddress.sin_port = htons(RESTAURANT_PORT);

    // Connect to server
    status = connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (status < 0) {
        perror("Connection to server failed");
        close(clientSocket);
        exit(-1);
    }

    // Prepare and send the REQUEST_DELIVERY command
    buffer[0] = REQUEST_DELIVERY;
    strncpy(&buffer[NAME_START], argv[1], NAME_SIZE);
    buffer[NUM_ITEMS_START] = totalItems;
    
    for (int i = 0; i < totalItems; ++i) {
        int itemIndex = atoi(argv[i + 2]);
        if (itemIndex < 1 || itemIndex > MENU_SIZE) {
            printf("Invalid item number: %d\n", itemIndex);
            close(clientSocket);
            exit(-1);
        }
        buffer[ITEMS_START + i] = itemIndex;
    }

    send(clientSocket, buffer, ITEMS_START + totalItems, 0);
    printf("CLIENT: Order sent to server, awaiting response.\n");

    // Receive total price from the server
    float totalPrice;
    int bytesReceived = recv(clientSocket, &totalPrice, sizeof(float), 0);
    printf("CLIENT: Received %d bytes from server.\n", bytesReceived);
    printf("Attempting dubious float conversion.\n");

    if (bytesReceived <= 0) {
        printf("CLIENT: Failed to receive response from server.\n");
        close(clientSocket);
        exit(-1);
    }

    printf("CLIENT: Got back response ORDER_RECEIVED, total $%.2f from server.\n", totalPrice);

    close(clientSocket);
    printf("CLIENT: Shutting down.\n");
    return 0;
}