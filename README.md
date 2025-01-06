# Restaurant Management System

This project simulates a restaurant's operations using multi-threading and a client-server model. It allows handling orders, assigning drivers for deliveries, and managing the restaurant's menu and driver statuses. The program is implemented in C with the use of `pthread` for concurrency and socket programming for communication. The project was a part of 'Introductin to Systems Programming' course taken at Carleton University.

---

## Features

1. **Order Management**
   - Customers can place orders, and they are queued for delivery.
   - Orders contain customer information, selected menu items, and their total price.

2. **Driver Management**
   - Drivers pick up orders from the queue and deliver them.
   - Driver statuses are updated in real-time:
     - `WAITING` - Ready to pick up an order.
     - `ON_ROUTE` - Delivering an order.
     - `DELIVERING` - Completing the delivery.
     - `RETURNING` - Returning to the restaurant.

3. **Queue System**
   - A thread-safe queue ensures orders are processed in the order they are received.

4. **Menu Management**
   - The restaurant has a predefined menu with items and prices.

5. **Concurrency**
   - Separate threads handle incoming requests, driver actions, and restaurant operations.

6. **Server Communication**
   - The server listens for requests, processes commands (e.g., place orders, print orders), and responds to clients.

---

## How It Works

### Main Components

- **Restaurant (`restaurant.h`)**
  - Manages the menu, order queue, and driver statuses.
  
- **Order Handler (`orderHandler.c`)**
  - Handles incoming order requests from customers via sockets.
  - Adds valid orders to the queue.

- **Driver Handler (`driverHandler.c`)**
  - Simulates driver actions, including picking up, delivering, and returning from orders.

- **Main Program (`main.c`)**
  - Initializes the restaurant and spawns threads for order handling and driver management.
  - Waits for threads to complete on shutdown.

---

## Getting Started

### Prerequisites

- GCC or a compatible C compiler.
- POSIX-compatible operating system for `pthread` support.

### Build Instructions

1. Open a terminal and navigate to the project directory.
2. Compile the program using the provided Makefile:
   ```bash
   make
   ```

### Run Instructions

1. Start the server:
   ```bash
   ./main
   ```
2. Interact with the server using a client program or manually send requests via tools like `telnet`.

---

## Code Overview

### Files

- `main.c`: Entry point, initializes the restaurant and threads.
- `restaurant.c`: Core restaurant functionality, including menu and queue management.
- `driverHandler.c`: Handles driver threads and their actions.
- `orderHandler.c`: Processes incoming order requests.
- `restaurant.h`: Defines key structures and constants.

### Key Structures

- **`Order`**: Represents a customer order, including items and total price.
- **`Driver`**: Tracks driver statuses and their current order.
- **`Queue`**: Thread-safe order queue.

### Thread Synchronization

- Mutexes (`pthread_mutex_t`) ensure safe access to shared resources, like the order queue.

---

## Example Output

When running, the program outputs real-time updates, such as:
- New orders received.
- Drivers picking up, delivering, and completing orders.
- Orders added to the queue or processed.

---

## Future Enhancements

- A graphical interface for better visualization.
- Expand functionality to include customer feedback and delivery tracking.

---
