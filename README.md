Name: FORAM KANTHARIA
Student Number: 101330616

List of Files:
FOLDER: assignment4.zip

Folders Inside Folder: 

1. a4p1:
Files inside:
1. restaurant.c
2. restaurant.h
3. main.c
4. Makefile 

2. a4p2:
Files inside:
1. restaurant.c
2. restaurant.h
3. main.c
4. command.c
5. customer.c
6. orderHandler.c
7. Makefile 

3. avp3:
Files inside:
1. restaurant.c
2. restaurant.h
3. main.c
4. command.c
5. customer.c
6. orderHandler.c
7. driverHandler.c
8. Makefile 


Purpose of each file, compilation and execution instructions:

a4p1 folder:

1. restaurant.h: The defs.h header file contains the constants and data types that the program must use

2. main.c: This file handles execution of the program.

3. restaurant.c: This file contains all the necessary functions like deliverOrder, printAllOrders etc.. used to execute this program.

4. Makefile: This file compiles each individual .c file into  corresponding object file linking them into one executable.

a4p2 folder:

1. restaurant.h: The defs.h header file contains the constants and data types that the program must use

2. main.c: This file handles execution of the program.

3. restaurant.c: This file contains all the necessary functions like deliverOrder, printAllOrders etc.. used to execute this program.

4. Makefile: This file compiles each individual .c file into  corresponding object file linking them into one executable.

5. customer.c: This file handles customer inputs and communicates with the server and outputs the order and other necessary details 

6. command.c: This file handles commands orders, deliver, drivers and shutdown

7. orderHandler.c: Tis file handles the server logic i.e. takes orders, print them, delivers them etc. It is the main file that allows client communication with the system and processes them.

avp3 folder: 
1. restaurant.h: The defs.h header file contains the constants and data types that the program must use

2. main.c: This file handles execution of the program.

3. restaurant.c: This file contains all the necessary functions like deliverOrder, printAllOrders etc.. used to execute this program.

4. Makefile: This file compiles each individual .c file into  corresponding object file linking them into one executable.

5. customer.c: This file handles customer inputs and communicates with the server and outputs the order and other necessary details 

6. command.c: This file handles commands orders, deliver, drivers and shutdown

7. orderHandler.c: This file handles the server logic i.e. takes orders, print them, delivers them etc. It is the main file that allows client communication with the system and processes them.

8. driverHandler.c: This file handles threading of drivers and the functionality of automatically handling of deliveries by driver.

Compilation and Execution:
1. Download the folder assignment4.zip
2. Extract all folders from the main folder
3. open your terminal and type cd name of directory where you stored the folders 

To run a4p1:
1. Once in the directory, type cd a4p1
2. Once in the a4p1 directory, type make
3. Then type ./a4
4. The program will now run and you can input your desired selections as per the options displayed 

To run a4p2:
1. Once in directory, type cd a4p2
2. Once in the a4p1 directory, type make
3. Then type ./a4 &
4. The program will now run and you can type ./customer <name> <item number> <item number> to add an order. If you want to print orders, drivers or deliver or shutdown type ./command orders to print orders, ./command drivers to print drivers, ./command deliver to deliver order, and ./command shutdown to shutdown the system 

To run avp3:
1. Once in directory, type cd avp3
2. Once in the avp3 directory, type make
3. Then type ./a4 and the program will start running with printing drivers that are waiting for an order
4. Open another terminal go to the avp3 directory same as earlier and then type make
5. Then type ./customer <name> <item name><item name> to add an order and then if you see the previous terminal, the order will automatically be delivered
6. If you need to run any commands, you can run it from the second terminal as well



