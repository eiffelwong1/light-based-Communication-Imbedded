all: sender receiver

sender:
	gcc -o sender -lwiringPi sender.c

receiver:
	gcc -o receiver -lwiringPi receiver.c

flash:
	gcc -o flash -lwiringPi flash.c

clean:
	rm sender receiver


ham:
	gcc -o hamming hamming.c
	./hamming
