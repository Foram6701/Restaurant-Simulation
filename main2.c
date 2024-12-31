#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>

#include "restaurant.h"

#include "orderHandler.c"




// This is where it all begins
int main() {
	Restaurant		goodEats;

	initRestaurant(&goodEats);

	// Spawn a thread to start the line coordinator
  	pthread_t  orderHandlerThread;
  	pthread_create(&orderHandlerThread, NULL, handleIncomingRequests, (void *)(&goodEats));
  	sleep(1);

  	// Wait for the line coordinator thread to complete (from a SHUT_DOWN command)
  	pthread_join(orderHandlerThread, NULL);
	

	printf("Freeing restaurant memory...\n");
	
	cleanupRestaurant(&goodEats);
	

  	printf("Restaurant Application Ended\n");

	
    
}
