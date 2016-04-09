#include <stdio.h>
#include "auxiliary.h"
#include <string.h>
#include <stdlib.h>

extern int fillEmpty (int index);
extern void testSudoku();

// reads path to problem file and to solution file
// displays current state of the sudoku
// if sudoku is incomplete, completes the solution,
// writes it to the solution file and displays it
// if command was ./sudoku test -- runs the test() method on files from
// directories: testprob and testsoln
int main(int argc, char** argv)
{
	if ((argv[1][0]=='t') && (argv[1][1]=='e') && (argv[1][2]=='s') && (argv[1][3]=='t')){
		printf("\nWill test sudoku: \n\n");
		testSudoku();
		exit(1);
	}
	// if solution is invalid or problem is invalid
	if( readSolution(argv[2])==-1 ||readProblem(argv[1]) ==-1)
	{
		displayStates();
		exit(0);
	}
	// displaying current sudoku grid
	printf("\n\nCurrent solution is: \n\n");
	displayArrTable();
	printf("\n\n");
	
	// display applicable states
	displayStates();

	// complete the solution
	if(state[2]) 
	{	
		printf("Please wait... Solving the sudoku...\n");
		// solve sudoku and display result
		if (fillEmpty(0)==1)
		{	
			printf("\nSuccessfully completed sudoku.\nTable is:\n\n");
			// display solution
			displayArrTable();	
			// write solution to file
			if (writeSolution(argv[2])==-1)
					printf("\nError writing solution to file.\n");
				else
					printf("\nWritten solution to file: %s\n" , argv[2]);
		} else
			printf("Sudoku is still not completed.\n"); // should not happen
	}	
}

