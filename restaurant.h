#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <stdio.h>
#include <pthread.h>

#define NUM_DRIVERS 3 // # of drivers at the store

#define MAX_ITEMS 50 // Maximum # of items in an order
#define MENU_SIZE 55 // Maximum # of items being sold at the store
#define NAME_SIZE 20 // Maximum size of a customer name

// Starting point (in bytes) of information sent via socket
#define NAME_START 1	   // Start of the name in the buffer
#define NUM_ITEMS_START 22 // Start of the number of items in the buffer
#define ITEMS_START 23	   // Start of the items in the buffer

// Driver status
#define OFF_LINE 0
#define WAITING 1
#define ON_ROUTE 2
#define DELIVERING 3
#define RETURNING 4

// Commands/Replies that a Customer sends to the lineCoordinator
#define REQUEST_DELIVERY 1 // Customer requuests delivery
#define ORDER_RECEIVED 2   // Reply from server indicating order has been received
#define PRINT_ORDERS 3     // Print all orders in the queue
#define PRINT_DRIVERS 4    // Print all drivers
#define DELIVER_ORDER 5    // Deliver an order (call the appropriate function)
#define SHUT_DOWN 6        // Shut down the server

#define TRUE 1
#define FALSE 0

#define RESTAURANT_IP "127.0.0.1"
#define RESTAURANT_PORT 6000

// This struct represents a menu item
typedef struct
{
	char *name;	 // name of the item
	float price; // cost of the item
} Item;

// This represents an order made by a customer
typedef struct
{
	char customerName[NAME_SIZE];
	Item items[MAX_ITEMS];
	int numItems;
	float totalPrice;
} Order;

typedef struct NodeType {
    struct NodeType *next;
    Order *data;  // Pointer to the order associated with this node
} Node;

typedef struct Queue {
    Node *head;
    Node *tail;
    pthread_mutex_t queue_mutex;  // Mutex for thread-safe operations
} Queue;





// This struct represents a Driver that delivers orders
typedef struct
{
	// TODO this is new
	Queue* orders;   // This will be a shared reference to the restaurant's orders queue
	unsigned char id; // Unique id for this driver
	// unsigned int				pid;	// Process ID for this driver
	unsigned char status; // OFF_LINE, WAITING, ON_DELIVERY
	float moneyCollected; // Total money accepted from all customers so far
	Order *order;		  // current order, or NULL if no order
} Driver;

typedef struct
{
	Queue orders;
	Item menu[MENU_SIZE];		 // the items that are sold at the store
	Driver drivers[NUM_DRIVERS]; // the drivers at the store
} Restaurant;

// These are the items being sold at the restaurant
static char *ITEM_NAMES[] = {
	"Chicken Burger", "Hamburger", "Cheeseburger", "Fish Burger", "Veggie Burger", "Chicken Wrap", "Beef Wrap",
	"Fish Wrap", "Veggie Wrap", "Chicken Salad", "Beef Salad", "Fish Salad", "Veggie Salad", "Fries", "Onion Rings",
	"Cheese Sticks", "Chicken Nuggets", "Fish Sticks", "Veggie Nuggets", "Coke", "Diet Coke", "Sprite", "Root Beer",
	"Orange Juice", "Apple Juice", "Milk", "Coffee", "Tea", "Water", "Chocolate Milk", "Vanilla Milk", "Strawberry Milk",
	"Chocolate Shake", "Vanilla Shake", "Strawberry Shake", "Chocolate Ice Cream", "Vanilla Ice Cream", "Strawberry Ice Cream",
	"Chocolate Cake", "Vanilla Cake", "Strawberry Cake", "Chocolate Muffin", "Vanilla Muffin", "Strawberry Muffin",
	"Chocolate Donut", "Vanilla Donut", "Strawberry Donut", "Chocolate Cookie", "Vanilla Cookie", "Strawberry Cookie",
	"Chocolate Brownie", "Vanilla Brownie", "Strawberry Brownie", "Chocolate Pie", "Vanilla Pie"};

static float ITEM_PRICES[] = {7.97, 1.98, 1.00, 20.97, 3.17, 17.97, 2.00, 3.07, 2.77, 5.39, 5.47, 3.88, 1.58,
							  4.47, 6.28, 2.38, 0.62, 1.97, 1.97, 0.59, 12.00, 0.52, 4.97, 0.97, 1.97, 2.08, 3.97, 2.97, 4.97, 1.77, 5.97,
							  6.27, 1.97, 3.27, 1.57, 4.97, 0.37, 1.87, 2.97, 4.27, 1.47, 5.97, 1.24, 3.27, 1.27, 1.67, 3.47, 1.97, 2.97,
							  3.97, 1.77, 1.57, 4.77, 2.47, 3.27};

void initRestaurant(Restaurant *rest);
void cleanupRestaurant(Restaurant *rest);

void initQueue(Queue *q);
void cleanUp(Queue *q);
void cleanupQueue(Queue *q);
void displayOrder(Order *order);
void displayOrdersInQueue(Queue *queue);
void displayDriver(Driver *driver);

void addToQueue(Queue *queue, Order *order);
Order *removeFromQueue(Queue *queue);

void printAllDrivers(Restaurant *rest);
void printAllOrders(Restaurant *rest);
void receiveOrder(Restaurant *rest, Order *order);
void deliverOrder(Restaurant *rest);
void simpleDeliverOrder(Restaurant *rest);

#endif
