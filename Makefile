a.out: main.c
	$(CC) main.c -o a.out -Wall -Wextra

clean:
	rm -vf *.out
