
all:
	gcc -o sensor_run sensor_run.c sensors.c -lwiringPi -lpthread
	./sensor_run

clean:
	rm -rf sensor_run
