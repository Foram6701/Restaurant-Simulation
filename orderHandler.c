#include "restaurant.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Helper function to send a response back to the client
void sendResponse(int clientSocket, const char* message) {
    if (message && strlen(message) > 0) {
        send(clientSocket, message, strlen(message), 0);
    }
}

void *handleIncomingRequests(void *x) {
    Restaurant *restaurant = (Restaurant *)x;
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    char buffer[1024]; // Buffer to handle responses
    int addrSize, bytesRcv, done = FALSE;
    int opt = 1;

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket < 0) {
        perror("Server socket creation failed");
        exit(-1);
    }

    // Set socket option to reuse address
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(serverSocket);
        exit(-1);
    }

    // Bind address to socket
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(RESTAURANT_PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Bind failed");
        close(serverSocket);
        exit(-1);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) < 0) {
        perror("Listen failed");
        close(serverSocket);
        exit(-1);
    }

    printf("SERVER: Ready to accept connections.\n");

    while (!done) {
        addrSize = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &addrSize);
        if (clientSocket < 0) {
            perror("Accept failed");
            continue;
        }

        bytesRcv = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRcv < 1) {
            perror("Receive failed");
            close(clientSocket);
            continue;
        }

        buffer[bytesRcv] = '\0'; // Null-terminate the received data
        printf("SERVER: Bytes received: %d\n", bytesRcv);

        switch (buffer[0]) {
            case REQUEST_DELIVERY: {
                printf("SERVER: Received delivery request.\n");

                // Extract customer name
                char customerName[NAME_SIZE];
                strncpy(customerName, &buffer[NAME_START], NAME_SIZE - 1);
                customerName[NAME_SIZE - 1] = '\0'; // Ensure null termination
                printf("SERVER: Customer name: %s\n", customerName);

                // Extract number of items
                int numItems = buffer[NUM_ITEMS_START];
                printf("SERVER: Number of items in order: %d\n", numItems);

                // Validate and create the order
                Order *order = (Order *)malloc(sizeof(Order));
                strncpy(order->customerName, customerName, NAME_SIZE);
                order->numItems = numItems;
                order->totalPrice = 0;
                int validOrder = 1;

                for (int i = 0; i < numItems; i++) {
                    int itemIndex = buffer[ITEMS_START + i];
                    if (itemIndex <= 0 || itemIndex > MENU_SIZE) {
                        printf("SERVER: Invalid item index: %d\n", itemIndex);
                        validOrder = 0;
                        break;
                    }
                    order->items[i] = restaurant->menu[itemIndex - 1];
                    order->totalPrice += order->items[i].price;
                }

                if (!validOrder) {
                    free(order); // Free invalid order
                    sendResponse(clientSocket, "Invalid order received. Order discarded.\n");
                    close(clientSocket);
                    break;
                }

                printf("SERVER: Displaying order\n");
                displayOrder(order);

                // Add order to the queue
                pthread_mutex_lock(&(restaurant->orders.queue_mutex));
                addToQueue(&restaurant->orders, order);
                pthread_mutex_unlock(&(restaurant->orders.queue_mutex));

                // Send confirmation to client
                char response[256];
                snprintf(response, sizeof(response), "Order received. Total price: $%.2f\n", order->totalPrice);
                sendResponse(clientSocket, response);
                break;
            }

            case PRINT_ORDERS: {
                printf("SERVER: Received print orders request.\n");
                pthread_mutex_lock(&(restaurant->orders.queue_mutex));
                printAllOrders(restaurant);
                pthread_mutex_unlock(&(restaurant->orders.queue_mutex));
                sendResponse(clientSocket, "Orders printed to server console.\n");
                break;
            }

            case PRINT_DRIVERS: {
                printf("SERVER: Received print drivers request.\n");
                printAllDrivers(restaurant);
                sendResponse(clientSocket, "Drivers status printed to server console.\n");
                break;
            }

            case DELIVER_ORDER: {
                printf("SERVER: Deliver order command ignored. Drivers handle delivery automatically.\n");
                break;
            }

            case SHUT_DOWN: {
                printf("SERVER: Received shutdown request.\n");
                for (int i = 0; i < NUM_DRIVERS; i++) {
                    restaurant->drivers[i].status = OFF_LINE;
                }
                pthread_mutex_lock(&(restaurant->orders.queue_mutex));
                cleanupQueue(&restaurant->orders);
                pthread_mutex_unlock(&(restaurant->orders.queue_mutex));
                done = TRUE;
                break;
            }

            default: {
                printf("SERVER: Unknown command received.\n");
                sendResponse(clientSocket, "Unknown command.\n");
                break;
            }
        }

        close(clientSocket);
        printf("SERVER: Closing client connection.\n");
    }

    close(serverSocket);
    printf("SERVER: Shutting down server socket.\n");

    pthread_exit(NULL); // Explicitly exit the thread
    return NULL;
}
