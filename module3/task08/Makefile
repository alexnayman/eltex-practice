all: producer consumer

producer: producer.c
	gcc producer.c -o producer

consumer: consumer.c
	gcc consumer.c -o consumer

clean:
	rm -f producer consumer

clean_ipc:
	ipcs -s | awk '/0x/ {print $$2}' | xargs -n1 ipcrm -s
