CFLAGS=-O0 -g -Wall -Werror -Wextra -std=c17

all: playerscore-from-guide-part1 playerscore-from-guide-part2
clean:
	rm -rf *.o playerscore-from-guide-part1 playerscore-from-guide-part2 a.out

playerscore-from-guide-part1: CFLAGS+=-DPART1
playerscore-from-guide-part1: playerscore-from-guide.c
playerscore-from-guide-part1:
	$(CC) $(CFLAGS) -o $@ playerscore-from-guide.c

playerscore-from-guide-part2: CFLAGS+=-DPART2
playerscore-from-guide-part2: playerscore-from-guide.c
playerscore-from-guide-part2:
	$(CC) $(CFLAGS) -o $@ playerscore-from-guide.c

