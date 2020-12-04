run: build/sol
	$< < input

check: test leaks

test: build/sol
	$< < example | grep -qx 'part 1: 2'
	$< < example | grep -qx 'part 2: 2'
	$< < input | grep -qx 'part 1: 192'
	$< < input | grep -qx 'part 2: 101'

leaks: build/sol
	leaks --quiet --atExit -- $< < input

build/sol: sol.c passport.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf build
