#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>

#include "orderHandler.c"
#include "driverHandler.c"

// This is where it all begins
int main() {
    Restaurant goodEats;

    // Initialize the restaurant
    initRestaurant(&goodEats);

    // Spawn a thread to start the line coordinator
    pthread_t orderHandlerThread;
    pthread_create(&orderHandlerThread, NULL, handleIncomingRequests, (void *)(&goodEats));
    sleep(1); // Give the order handler some time to initialize

    // Spawn threads for the drivers
    pthread_t driverThreads[NUM_DRIVERS];
    for (int i = 0; i < NUM_DRIVERS; i++) {
        pthread_create(&driverThreads[i], NULL, handleDriver, (void *)(&goodEats.drivers[i]));
        printf("Driver thread %d started.\n", i + 1);
    }

    // Wait for the line coordinator thread to complete (from a SHUT_DOWN command)
    pthread_join(orderHandlerThread, NULL);

    // Wait for all driver threads to finish
    for (int i = 0; i < NUM_DRIVERS; i++) {
        pthread_join(driverThreads[i], NULL);
        printf("Driver thread %d finished.\n", i + 1);
    }

    // Clean up resources
    printf("Freeing restaurant memory...\n");
    cleanupRestaurant(&goodEats);

    printf("Restaurant Application Ended\n");
    return 0;
}
