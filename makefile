CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g
COV_FLAGS = --coverage

UTIL_OBJECTS = utilities/hash_table.o \
								utilities/hash_table_iterator.o \
								utilities/linked_list.o \
								utilities/linked_list_iterator.o \
								utilities/utils.o

COV_OBJECTS = utilities/hash_table.c \
								utilities/hash_table_iterator.c \
								utilities/linked_list.c \
								utilities/linked_list_iterator.c \
								utilities/utils.c

all: backend_tests

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

db: main/db.c main/db.h utilities/common.h
	$(CC) $(CFLAGS) -c $< -o $@

cart: main/cart.c main/cart.h utilities/common.h
	$(CC) $(CFLAGS) -c $< -o $@

io: main/io.c main/io.h utilities/utils.h
	$(CC) $(CFLAGS) -c $< -o $@
	
ui: main/ui.c main/db.c main/cart.c main/io.c $(UTIL_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

backend_tests: backend_tests.c main/db.c $(UTIL_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ -lcunit	

utilities/%.o: utilities/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: backend_tests
	./backend_tests

mem: backend_tests
	valgrind --leak-check=full ./backend_tests

memu: ui
	valgrind --leak-check=full ./ui


cov:
	$(CC) $(CFLAGS) $(COV_FLAGS) $(COV_OBJECTS) main/db.c backend_tests.c -o backend_tests_cov -lcunit
	./backend_tests_cov
	gcov -abcf backend_tests_cov-backend_tests.gcno
	gcovr -r . --filter main/db\.c

clean:
	rm -f utilities/*.o
	rm -f main/*.o
	rm -f backend_tests db io ui cart
	rm -f *.gcda *.gcno *.gcov
	rm -f backend_tests_cov
	rm -f cart
	rm -f Doxyfile
