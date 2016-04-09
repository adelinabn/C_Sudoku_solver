#include "auxiliary.h"
#include <dirent.h> 
#include <stdio.h> 

extern int fillEmpty (int index);
void testSudoku();
int readSolve(char* solnfile, char *probfile);

// reads pairs of problem-solution files from testprob and testsoln local directories respectively
// if grid is incomplete and manages to complete it, will overwrite the solution in the file from
// testsoln directory, otherwise, outputs appropriate message.
// displays the state of the sudoku after reading / solving it
void testSudoku()
{
	DIR           *dprob;
  	struct dirent *dirprob;
	char* 		   probfile;
	char*          solnfile;
    dprob = opendir("testprob");
    char cwd[1024]; // current directory
    getcwd(cwd, sizeof(cwd));
    if (dprob) {
	    while ((dirprob = readdir(dprob)) != NULL)
	    {	
	    	probfile = 	dirprob->d_name;
		    solnfile =  concat("soln_", probfile);
		    // exclude hidden files, which all start with '.'
		    if ((solnfile[0]!='.') && (probfile[0]!='.')){
		    	// get the absolute paths of files
		    	char* s = concat(cwd, concat ("/testsoln/",solnfile));
		    	char* p= concat(cwd, concat ("/testprob/",probfile));
		    	// process and display results
		    	printf("____________________________________________________________________\n");
		    	printf("%s\t\t%s\n", solnfile, probfile);
		    	printf("--------------------------------------------------------------------\n");
		    	int outcome = readSolve(s,p);
		      	printf("test:\t\t");
				switch (outcome)
				      	 {
				      	 	case -1: printf("Could not solve sudoku.\n"); break;
				      	 	case 0 : printf("Invalid problem or solution\n"); break;
				      	 	case 1 : printf("Solution is valid\n"); break;
				      	 	case 2 : printf("Successfully solved sudoku: \n");
				      	 			 
				      	 }
				printf("\n");
				if(outcome!=0) // if solution and problem ar valid
		      	    displayArrTable();
		    }
	    }
    closedir(dprob);
    }
}


// tries to read a sudoku problem from probfile and write the solution to solnfile
// returns 1 if sudoku is solved
// 		   2 if sudoku was solved and written to solnfile
//        -1 if sudoku could not be solved
//         0 if the problem was invalid or solution was in wrong format
int readSolve(char* solnfile, char *probfile)
{	
	initialiseState();
	if( readSolution(solnfile)==-1 || readProblem(probfile) ==-1)
	{
		displayStates(); // displays state of the sudoku after reading the problem & solution
		return 0;
	}
	if (state[2]) // if incomplete, but problem is valid
	{
		displayStates();
		printf("\n ...Solving sudoku...\n");
		if (fillEmpty(0)==1) // sudoku solved successfully
		{
			writeSolution(solnfile);
			return 2;
		} 
	}
	if (state[3]==1)
		return 1; // sudoku is already solved
	return -1;    // could not be solved
}