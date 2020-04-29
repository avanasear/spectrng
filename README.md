# SpectRNG

*Leaking data gathered via the Spectre vulnerability through the CPU's random number generator*

*The spectre portion of the code is a modified version of the code published in P. Kocher et al., “Spectre Attacks: Exploiting Speculative Execution,” in 2019 IEEE Symposium on Security and Privacy (SP), May 2019, pp. 1–19, doi: 10.1109/SP.2019.00002.*


Run `make` to compile both binaries: `spectrng` and `readsecret`

To run the PoC, run `spectrng` in one terminal, and `readsecret` in another terminal immediately after `spectrng` prints out 'The Magic Words are Squeamish Ossifrage.' If `spectrng` prints anything else, or fails to print anything and exits, just run it again. Shit happens.
