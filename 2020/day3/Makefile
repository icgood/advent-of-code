run: build/sol
	build/sol < input

check: build/sol
	leaks --atExit -- build/sol < input

build/sol: sol.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf build
