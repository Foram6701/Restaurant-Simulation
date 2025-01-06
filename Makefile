all: a4 command customer

a4: main.o restaurant.o
	gcc -o a4 main.o restaurant.o

command: command.c 
	gcc -o command command.c

customer: customer.c
	gcc -o customer customer.c

main.o: main.c restaurant.h orderHandler.c
	gcc -c main.c

restaurant.o: restaurant.c restaurant.h
	gcc -c restaurant.c

driverHandler.o: driverHandler.c restaurant.h
	gcc -c driverHandler.c	

clean:
	rm -f a4 *.o