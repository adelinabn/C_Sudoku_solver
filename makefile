

sudoku : sudoku.o solver.o auxiliary.o check.o test.o
	gcc sudoku.o solver.o auxiliary.o check.o test.o -o sudoku -std=c99 -Wall -Wextra -g -lm

sudoku.o : sudoku.c  g_variables.h auxiliary.h
	gcc sudoku.c -c -o sudoku.o -std=c99 -Wall -Wextra -g

solver.o : solver.c  g_variables.h
	gcc solver.c -c -o solver.o -std=c99 -Wall -Wextra -g

auxiliary.o : auxiliary.c auxiliary.h
	gcc auxiliary.c -c -o auxiliary.o -std=c99 -Wall -Wextra -g 

check.o : check.c auxiliary.h
	gcc check.c -c -o check.o -std=c99 -Wall -Wextra -g

test.o : test.c auxiliary.h 
	gcc test.c -c -o test.o -std=c99 -Wall -Wextra -g

