# STORE

### make backend_tests:
To compile and run tests for the cart.c and db.c *(note: io.c and ui.c is not tested in similar manner as the earlier .c files beacuse they manage user input)*
### make mem:
To compile and run valgrind tests for cart.c and db.c 
### make cov:
To run coverage tests for cart.c and db.c 
### make main:
To compile the program.
### make run-main
To compile and run the program *(manually, meaning entrying prompts by hand)*
### make run-file
To compile and run the program *(automated, change the .txt file by going to the make file and entering the path to the file at line 4 {INPUT_FILE = filename.txt})*

# Design
- **ui.c** *(User Interface)*: Displays the menu and lets the user choose actions
- **io.c** *(Input/Output)*: Gets user input, such as merchandise names, descriptions, and prices
- **db.c** *(Storage)*: Manages merchandise and stock
- **cart.c** *(Shopping Carts)*: Handles carts, checkouts and updates stock *(if needed)*

# Coverage:
db.c has 100 % line coverage and cart.c has 97% line coverage. We used gcov to obtain these numbers using -abcf and -r flags.
