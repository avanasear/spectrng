# SpectRNG

*Leaking data gathered via the Spectre vulnerability through the CPU's random number generator*

Run `make` to compile both binaries: `spectrng` and `readsecret`

To run the PoC, run `spectrng` in one terminal, and `readsecret` in another terminal immediately after `spectrng` prints out 'The Magic Words are Squeamish Ossifrage.' If `spectrng` prints anything else, or fails to pring anything and exits, just run it again. Shit happens.
