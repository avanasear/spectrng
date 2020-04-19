CC=gcc
CFLAGS="-mrdrnd -march=native"

all: src/main.c
	$(CC) src/spectre.c -o spectrng -mrdrnd -march=native

clean: spectrng
	rm spectrng
