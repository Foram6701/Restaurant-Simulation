#include "restaurant.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Function that each Driver thread executes.
 * Drivers automatically handle orders based on their status.
 */
void *handleDriver(void *x) {
    Driver *d = (Driver *)x;
    Queue *orders = d->orders;

    while (d->status != OFF_LINE) {
        // Simulate a delay (e.g., driving time) between actions
        sleep(rand() % 5 + 4); // Random sleep between 4 and 8 seconds

        switch (d->status) {
            case WAITING: {
                // Check for an available order in the queue
                pthread_mutex_lock(&(orders->queue_mutex)); // Lock the queue for thread-safe access
                if (orders->head == NULL) {
                    pthread_mutex_unlock(&(orders->queue_mutex)); // Unlock the queue
                    printf("Driver %d is waiting for an order.\n", d->id);
                } else {
                    // Pick up an order from the queue
                    d->order = removeFromQueue(orders);
                    pthread_mutex_unlock(&(orders->queue_mutex)); // Unlock the queue
                    d->status = ON_ROUTE; // Change status to ON_ROUTE
                    printf("Driver %d picked up an order and is now on route.\n", d->id);
                }
                break;
            }

            case ON_ROUTE: {
                // Start delivering the picked-up order
                d->status = DELIVERING;
                printf("Driver %d is delivering the order.\n", d->id);
                break;
            }

            case DELIVERING: {
                // Complete the delivery and update the driver's collected money
                if (d->order) {
                    d->moneyCollected += d->order->totalPrice; // Add the order price to the driver's total earnings
                    free(d->order); // Free the memory for the completed order
                    d->order = NULL;
                }
                d->status = RETURNING; // Change status to RETURNING
                printf("Driver %d has delivered the order and is returning.\n", d->id);
                break;
            }

            case RETURNING: {
                // Return to the restaurant and get ready for the next order
                d->status = WAITING; // Change status to WAITING
                printf("Driver %d has returned to the restaurant and is ready for new orders.\n", d->id);
                break;
            }

            default: {
                printf("Driver %d encountered an unknown status.\n", d->id);
                break;
            }
        }
    }

    // Clean up any remaining order if the driver goes offline
    if (d->order) {
        free(d->order);
        d->order = NULL;
    }
    printf("Driver %d is now offline.\n", d->id);
    return NULL;
}
