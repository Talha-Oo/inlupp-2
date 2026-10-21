CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g
COV_FLAGS = --coverage
INPUT_FILE = sample-texts/webstoretest.txt

DP_UTIL_OBJECTS = utilities/hash_table.o utilities/hash_table_iterator.o utilities/linked_list.o utilities/linked_list_iterator.o utilities/utils.o

DP_COV_OBJECTS = utilities/hash_table.c utilities/hash_table_iterator.c utilities/linked_list.c utilities/linked_list_iterator.c utilities/utils.c

DP_UTIL_OBJECTS = utilities/dp_ht/dp_hash_table.o utilities/dp_ht/dp_hash_table_iterator.o utilities/linked_list.o utilities/linked_list_iterator.o utilities/utils.o

DP_COV_OBJECTS = utilities/dp_ht/dp_hash_table.c utilities/dp_ht/dp_hash_table_iterator.c utilities/linked_list.c utilities/linked_list_iterator.c utilities/utils.c

all: backend_tests

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

db: src/db.c src/db.h utilities/common.h
	$(CC) $(CFLAGS) -c $< -o $@

cart: src/cart.c src/cart.h utilities/common.h
	$(CC) $(CFLAGS) -c $< -o $@

io: src/io.c src/io.h utilities/utils.h
	$(CC) $(CFLAGS) -c $< -o $@
	
ui: src/ui.c src/db.c src/cart.c src/io.c $(DP_UTIL_OBJECTS)
	$(CC) $(CFLAGS) -c $^ -o $@

backend_tests: backend_tests.c src/db.c src/cart.c $(DP_UTIL_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ -lcunit	

main: main.c src/ui.c src/db.c src/cart.c src/io.c $(DP_UTIL_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

utilities/%.o: utilities/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: backend_tests
	./backend_tests

mem: backend_tests
	valgrind --leak-check=full ./backend_tests

memu: ui
	valgrind --leak-check=full ./ui

cov:
	$(CC) $(CFLAGS) $(COV_FLAGS) $(DP_COV_OBJECTS) src/db.c src/cart.c backend_tests.c -o backend_tests_cov -lcunit
	./backend_tests_cov
	gcov -abcf backend_tests_cov-backend_tests.gcno
	gcovr -r . --filter 'src/db\.c' --filter 'src/cart\.c'


run-main: main
	./main

run-file: main
	./main < $(INPUT_FILE)

mem-file: main
	valgrind --leak-check=full ./main < $(INPUT_FILE)

massif: main
	valgrind --tool=massif  --massif-out-file=massif.file.out ./main < $(INPUT_FILE)
	ms_print massif.file.out > profs/YOO_MEM.txt

prof: main
	gcc $(CFLAGS) -pg main.c src/ui.c src/db.c src/cart.c src/io.c $(DP_COV_OBJECTS) -o main_prof
	./main_prof < $(INPUT_FILE)
	gprof main_prof gmon.out > profs/profile.txt
	more profs/profile.txt
	
prof_freq:
	gcc -Wall -Wextra -Wpedantic -g -pg utilities/hash_table.c utilities/hash_table_iterator.c utilities/freq_count.c -o freq_count 
	./freq_count sample-texts/1k-long-words.txt
	gprof freq_count gmon.out > profile.txt
	more profile.txt

clean:
	rm -f utilities/*.o
	rm -f src/*.o
	rm -f utilities/dp_ht/*.o
	rm -f utilities/dp_ht/dp_hash_table_tests
	rm -f backend_tests db io ui cart main
	rm -f *.gcda *.gcno *.gcov *.out
	rm -f utilities/*.gcda utilities/*.gcno utilities/*.gcov
	rm -f src/*.gcda src/*.gcno src/*.gcov
	rm -f backend_tests_cov
	rm -f Doxyfile