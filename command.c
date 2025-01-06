#include "restaurant.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


// Function to trim leading/trailing spaces
char *trimWhitespace(char *str) {
    char *end;

    // Trim leading spaces
    while (*str == ' ') str++;

    // Trim trailing spaces
    if (*str == '\0') return str; // All spaces

    end = str + strlen(str) - 1;
    while (end > str && *end == ' ') end--;

    // Null-terminate the trimmed string
    *(end + 1) = '\0';

    return str;
}

// Function to convert input to lowercase for comparison
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + ('a' - 'A');
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command>\n", argv[0]);
        printf("Available commands:\n");
        printf("  shutdown  - Shut down the server\n");
        printf("  orders    - Print all orders in the queue\n");
        printf("  drivers   - Print all drivers\n");
        printf("  deliver     - Deliver an order\n");
        exit(-1);
    }

    // Combine all arguments into a single command string
    char command[256] = "";
    for (int i = 1; i < argc; i++) {
        strcat(command, argv[i]);
        if (i < argc - 1) strcat(command, " "); // Add space between arguments
    }

    trimWhitespace(command); // Trim any extra spaces
    toLowerCase(command);    // Convert input to lowercase

    int clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[1];
    int status;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket < 0) {
        perror("Socket creation failed");
        exit(-1);
    }

    // Setup address
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

    // Map command to appropriate byte
    if (strcmp(command, "shutdown") == 0) {
        buffer[0] = SHUT_DOWN;
        printf("Sending shutdown command to server.\n");
    } else if (strcmp(command, "orders") == 0) {
        buffer[0] = PRINT_ORDERS;
        printf("Sending print orders command to server.\n");
    } else if (strcmp(command, "drivers") == 0) {
        buffer[0] = PRINT_DRIVERS;
        printf("Sending print drivers command to server.\n");
    } else if (strcmp(command, "deliver") == 0) {
        buffer[0] = DELIVER_ORDER;
        printf("Sending deliver order command to server.\n");
    } else {
        printf("Invalid command: %s\n", command);
        printf("Available commands:\n");
        printf("  shutdown - Shut down the server\n");
        printf("  orders   - Print all orders in the queue\n");
        printf("  drivers  - Print all drivers\n");
        printf("  deliver  - Deliver an order\n");
        close(clientSocket);
        exit(-1);
    }

    // Send command
    if (send(clientSocket, buffer, 1, 0) < 0) {
        perror("Failed to send command");
        close(clientSocket);
        exit(-1);
    }

    // Handle response if necessary
    if (buffer[0] == PRINT_ORDERS || buffer[0] == PRINT_DRIVERS || buffer[0] == DELIVER_ORDER) {
        char response[1024]; // Large buffer for receiving response
        int bytesReceived = recv(clientSocket, response, sizeof(response) - 1, 0);
        if (bytesReceived > 0) {
            response[bytesReceived] = '\0'; // Null-terminate the response
            printf("Server Response:\n%s\n", response);
        } else {
            printf("No response received from server.\n");
        }
    }

    // Close the socket
    close(clientSocket);

    // Final message for shutdown
    if (buffer[0] == SHUT_DOWN) {
        printf("CLIENT: Shutting down.\n");
    }

    return 0;
}