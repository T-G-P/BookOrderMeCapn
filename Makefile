CFLAGS = -Wall -g -lpthread

all: BookOrder Consumer

BookOrder: manager.o parse.o producer.o queue.o
	echo "BO"
	gcc $(CFLAGS) manager.o parse.o producer.o queue.o -o BookOrder

manager.o: manager.c
	gcc $(CFLAGS) -c manager.c

parse.o: parse.c
	gcc $(CFLAGS) -c parse.c

producer.o: producer.c
	gcc $(CFLAGS) -c producer.c

queue.o: queue.c
	gcc $(CFLAGS) -c queue.c

Consumer: consumer.o parse.o queue.o
	gcc $(CFLAGS) consumer.o parse.o queue.o -o Consumer

consumer.o: consumer.c
	gcc $(CFLAGS) -c consumer.c
clean:
	rm -f BookOrder
	rm -f Consumer
	rm -f *.o
