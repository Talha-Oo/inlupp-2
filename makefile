CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g -Iutilities
COV_FLAGS = --coverage

UTIL_OBJECTS = utilities/hash_table.o \
								utilities/hash_table_iterator.o \
								utilities/linked_list.o \
								utilities/linked_list_iterator.o \
								utilities/utils.o

all: backend_tests

backend_tests: backend_tests.c db.c $(UTIL_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ -lcunit	

utilities/%.o: utilities/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: backend_tests
	./backend_tests

clean:
	rm -f utilities/*.o
	rm -f backend_tests
	rm -f *.gcda *.gcno *.gcov
