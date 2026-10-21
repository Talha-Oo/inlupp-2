CC=gcc
FLAGS=-Wall -Wextra -Wpedantic -g
COV_FLAG = --coverage 

HASH_OBJECTS=dp_hash_table.o dp_hash_table_iterator.o
LIST_OBJECTS=linked_list.o linked_list_iterator.o
FREQ_OBJECTS=dp_hash_table.o dp_hash_table_iterator.o freq_count.o

COV_HASH_OBJECTS=dp_hash_table.c dp_hash_table_iterator.c
COV_LIST_OBJECTS=linked_list.c linked_list_iterator.c

all: dp_hash_table_tests

dp_hash_table_tests: $(HASH_OBJECTS) dp_hash_table_tests.c
	$(CC) $(FLAGS) $^ -o $@ -lcunit

linked_list_tests: $(LIST_OBJECTS) linked_list_tests.c
	$(CC) $(FLAGS) $^ -o $@ -lcunit

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@


freq: $(FREQ_OBJECTS) 
	$(CC) $(FLAGS) $^ -o freq_count

test: dp_hash_table_tests
	./dp_hash_table_tests 
	

tests: dp_hash_table_tests
	./dp_hash_table_tests 


memtest: dp_hash_table_tests
	valgrind ./dp_hash_table_tests


cov:
	$(CC) $(FLAGS) $(COV_FLAG) $(COV_HASH_OBJECTS) dp_hash_table_tests.c -o dp_hash_table_tests_cov -lcunit
	./dp_hash_table_tests_cov
	gcov -abcf dp_hash_table_tests_cov-dp_hash_table.gcno
	gcov -abcf dp_hash_table_tests_cov-dp_hash_table_iterator.gcno
	gcovr -r .

	$(CC) $(FLAGS) $(COV_FLAG) $(COV_LIST_OBJECTS) linked_list_tests.c -o linked_list_tests_cov -lcunit
	./linked_list_tests_cov
	gcov -abcf linked_list_tests_cov-linked_list.gcno
	gcov -abcf linked_list_tests_cov-linked_list_iterator.gcno
	gcovr -r .

prof:
	gcc -pg dp_hash_table.c dp_hash_table_iterator.c freq_count.c -o freq_count
	./freq_count sample-texts/16k-words.txt
	gprof freq_count gmon.out > profile.txt
	more profile.txt

massif:
	valgrind --tool=massif ./freq_count tbontb.txt
	
clean:	
	rm -f *.o *.gcda *.gcno
	rm -f *.out a.out
	rm -f dp_hash_table_tests linked_list_tests freq_count
	rm -f linked_list_tests_cov dp_hash_table_tests_cov