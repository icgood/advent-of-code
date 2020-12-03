run: sol
	./sol < input

sol: sol.c
	$(CC) -o $@ $^

clean:
	rm -f sol

check: sol
	leaks --atExit -- ./sol < input
