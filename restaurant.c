#include "restaurant.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void initQueue(Queue *q) {
    q->head = q->tail = NULL;
    pthread_mutex_init(&(q->queue_mutex), NULL); // Initialize mutex
}


/*
* Clean up the queue, freeing all allocated memory.
*/
void cleanUp(Queue* q) {
    Node* current = q->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp->data); // Free the associated Order data
        free(temp);
    }
    q->head = NULL;
    q->tail = NULL;
}

/*
* Display all orders in the queue
*/
void displayOrdersInQueue(Queue* queue) {
    Node* current = queue->head;
    if (!current) {
        printf("No orders in the queue.\n");
        return;
    }    
    printf("Orders in Queue:\n");
    while (current != NULL) {
        displayOrder(current->data);
        current = current->next;
    }
}

/*
* Add an order to the back of the queue
*/
void addToQueue(Queue* queue, Order* order) {
    // Allocate a new node
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = order;
    newNode->next = NULL; // Ensure the new node points to NULL

    if (queue->tail != NULL) {
        queue->tail->next = newNode; // Add the new node to the end of the queue
    }
    queue->tail = newNode; // Update the tail to the new node

    if (queue->head == NULL) {
        queue->head = newNode; // If the queue was empty, set the head
    }
}


/*
* Remove an order from the front of the queue and return it
*/
/*
* Removes an order from the front of the queue and returns it
*/
Order* removeFromQueue(Queue* queue) {
    if (queue->head == NULL) {
        printf("Queue is empty, no order to remove.\n");
        return NULL;
    }

    // Remove the head node
    Node* temp = queue->head;
    Order* order = temp->data;      // Retrieve the order
    queue->head = queue->head->next; // Move the head to the next node

    // If the queue becomes empty, set the tail to NULL
    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    free(temp); // Free the memory for the old head node
    return order; // Return the removed order
}


void displayOrder(Order* order){
  printf("\nCustomer: %s\n\n", order->customerName);
  for (int i=0; i<order->numItems; i++){    
    printf("Item %2d: %-20s $%7.2f\n", i, order->items[i].name, order->items[i].price);

  }
  printf("\n%-30s $%7.2f\n\n", "Total", order->totalPrice);
}

void displayDriver(Driver* driver){
  printf("Driver %d\n", driver->id);
  switch(driver->status){
    case OFF_LINE:
      printf("Status: Off Line\n");
      break;
    case WAITING:
      printf("Status: Waiting\n");
      break;
    case ON_ROUTE:
      printf("Status: On Route\n");
      break;
    case DELIVERING:
      printf("Status: Delivering\n");
      break;
    case RETURNING:
      printf("Status: Returning\n");
      break;

  }
  printf("Money Collected: %.2f\n", driver->moneyCollected);
  if (driver->order != NULL){
    displayOrder(driver->order);
  }else{
    printf("No current order\n");
  }
}



/*
* Initialize the restaurant.
* struct is provided for reference.

typedef struct{
	Queue  orders;
	Item   menu[MENU_SIZE];  // the items that are sold at the store
	Driver drivers[NUM_DRIVERS];  // the drivers at the store
} Restaurant;*/

void initRestaurant(Restaurant* rest){
  // initialize the queue
  initQueue(&rest->orders);
  // initialize the menu
  for (int i = 0; i < MENU_SIZE; ++i){
    rest->menu[i].name = ITEM_NAMES[i];
    rest->menu[i].price = ITEM_PRICES[i];
  }
  for (int i = 0; i < NUM_DRIVERS; i++) {
        rest->drivers[i].orders = &(rest->orders);
        rest->drivers[i].status = WAITING;
        rest->drivers[i].moneyCollected = 0.0f;
        rest->drivers[i].order = NULL;
        rest->drivers[i].id = i + 1;
    }
}

void cleanupRestaurant(Restaurant *rest) {
    printf("Cleaning up queue...\n");
    cleanupQueue(&(rest->orders)); // Free all orders in the queue

    // Free orders assigned to drivers
    for (int i = 0; i < NUM_DRIVERS; ++i) {
        if (rest->drivers[i].order != NULL) {
            free(rest->drivers[i].order); // Free any undelivered orders
            rest->drivers[i].order = NULL;
        }
    }
}
void cleanupQueue(Queue *q) {
    pthread_mutex_destroy(&(q-> queue_mutex)); // Destroy mutex
}


void receiveOrder(Restaurant* rest, Order* order){
  printf("Order received from customer\n");
  displayOrder(order);
  addToQueue(&rest->orders, order);
}

/*
* Drivers that are waiting will check if there is an order in the queue.
* If there is an order, the driver will pick it up and change status to ON_ROUTE
* If there is no order, the driver will remain in the WAITING status
* If a driver is ON_ROUTE, then they deliver the Order, free the Order memory
* and collect the money. They the change their status to DELIVERING, followed by RETURNING.
*
* (We could probably get rid of the DELIVERING status and just go from 
* ON_ROUTE to RETURNING, but we will leave it as is.)
*/
void deliverOrder(Restaurant *rest)
{
	for (int i = 0; i < NUM_DRIVERS; i++)
	{
		switch (rest->drivers[i].status)
		{
		case OFF_LINE:
			printf("Driver %d is off line\n", i);
			break;
		case WAITING:
			printf("Driver %d picking up order\n", i);
			Order *order = removeFromQueue(&rest->orders);
			if (order == NULL)
			{
				printf("No orders in queue\n");
				break;
			}
			rest->drivers[i].order = order;
			rest->drivers[i].status = ON_ROUTE;
			printf("Order assigned to driver %d\n\n", i);
			break;
		case ON_ROUTE:
			printf("Driver %d delivering order\n\n", i);
			displayOrder(rest->drivers[i].order);
			printf("\n");
			rest->drivers[i].status = DELIVERING;
      rest->drivers[i].moneyCollected += rest->drivers[i].order->totalPrice;
			free(rest->drivers[i].order);
			rest->drivers[i].order = NULL;
			break;
		case DELIVERING:
			printf("Driver %d returning\n\n", i);
			rest->drivers[i].status = RETURNING;
			break;
		case RETURNING:
			printf("Driver %d is back\n\n", i);
			rest->drivers[i].status = WAITING;
			break;
		}
	}
}

/*
* A simple version of deliverOrder that skips all the Driver nonsense,
* for testing purposes.
*/
void simpleDeliverOrder(Restaurant *rest)
{
	Order *order = removeFromQueue(&rest->orders);
	if (order == NULL)
	{
		printf("No orders in queue\n");
		return;
	}
	printf("Order being delivered:\n");
	displayOrder(order);
	free(order);
}

void printAllDrivers(Restaurant *rest)
{
	printf("\nDrivers:\n");
	for (int i = 0; i < NUM_DRIVERS; i++)
	{
		displayDriver(&rest->drivers[i]);
		printf("\n");
	}
}

void printAllOrders(Restaurant *rest)
{
  printf("\n");
	displayOrdersInQueue(&rest->orders);
}