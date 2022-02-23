all:
	sender receiver

sender:
	gcc -o sender -lwiringPi sender.c

receiver:
	gcc -o receiver -lwiringPi receiver.c

clean:
	rm sender receiver
