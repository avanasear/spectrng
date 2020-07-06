CC=gcc
CFLAGS="-mrdrnd -march=native"

all: src/spectre.c src/readsecret.c src/send.c src/recv.c
	$(CC) src/spectre.c -o spectrng -mrdrnd -lrt -march=native
	$(CC) src/readsecret.c -o readsecret -mrdrnd -march=native
	$(CC) src/send.c -o send -mrdrnd -lrt -march=native
	$(CC) src/recv.c -o recv -mrdrnd -lrt -march=native

clean: spectrng readsecret
	rm spectrng
	rm readsecret
	rm send
	rm recv
