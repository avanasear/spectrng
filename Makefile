CC=gcc
CFLAGS="-mrdrnd -march=native"

all: src/spectre.c src/readsecret.c
	$(CC) src/spectre.c -o spectrng -mrdrnd -lrt -march=native
	$(CC) src/readsecret.c -o readsecret -mrdrnd -march=native

clean: spectrng readsecret
	rm spectrng
	rm readsecret
