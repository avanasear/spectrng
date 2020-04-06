CC=gcc
CFLAGS="-lcrypto"

all: src/main.c
	$(CC) -o spectrng src/main.c $(CFLAGS)

clean: spectrng
	rm spectrng
