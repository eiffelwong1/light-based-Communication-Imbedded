all: sender receiver

sender:
	gcc -o sender -lwiringPi sender.c

receiver:
	gcc -o receiver -lwiringPi receiver.c

flash:
	gcc -o flash -lwiringPi flash.c

hamming:
	rm hamming
	gcc -o hamming hamming.c

clean:
	rm sender receiver
