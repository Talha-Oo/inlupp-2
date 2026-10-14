CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g
COV_FLAGS = --coverage

UTIL_OBJECTS = utilities/hash_table.o \
								utilities/hash_table_iterator.o \
								utilities/linked_list.o \
								utilities/linked_list_iterator.o \
								utilities/utils.o

all: backend_tests

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

db: main/db.c main/db.h utilities/common.h
	$(CC) $(CFLAGS) -c $< -o $@

io: main/io.c main/io.h utilities/utils.h
	$(CC) $(CFLAGS) utilities/utils.c $< -o $@

backend_tests: backend_tests.c main/db.c $(UTIL_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ -lcunit	

utilities/%.o: utilities/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: backend_tests
	./backend_tests

mem: backend_tests
	valgrind --leak-check=full ./backend_tests

clean:
	rm -f utilities/*.o
	rm -f backend_tests db io
	rm -f *.gcda *.gcno *.gcov:
	rm -f utilities/*.o
	rm -f backend_tests db io
	rm -f *.gcda *.gcno *.gcov
