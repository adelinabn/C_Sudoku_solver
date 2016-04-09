#include "auxiliary.h"

// will update the table, rows , clms, boxes arrays with that value
// returns -1 if value is already in one of the above
int updateCell (int r, int c, int value){
	table[r*dimtable+c]=value;
	int* pr= rows[r].values;
	int* pc = clms[c].values;
	int* pb = boxes[(r-1)/boxSize*boxSize+(c-1)/boxSize].values;
	//check possible value is not already within row/clm/box
	if ((pr[value]==1) || (pc[value]==1) || (pb[value]==1))
		return -1;
	//if possible value satisfies its context
	pr[value]=1;
	pc[value]=1;
	pb[value]=1;
	return 1;
}


// reads the number of cages into global nrOfCages
// followed by n descriptions of cages which it stores within the cages array
// returns -1 if it fails to correctly fill in all cages
int readProblem(char* probFile){

	FILE* f = fopen(probFile, "r");
	if(f == 0) 
		return setState(0,"Error opening problem file\n");
	if (!fscanf(f,"%d", &nrOfCages)){
		fclose(f);
		return setState(0,"Error reading the number of cages.\n");
	}	
	//create table for storing description of cages
	cages = (Cage *)malloc(sizeof(Cage)*nrOfCages);
	int r =	readCageDescriptions(f);
	fclose(f);
	return r;
}


// reads sudoku grid dimension and populates the array table starting with value at [1][1]
// returns 1 if all cells are completed or solution is incomplete and it makes sense
// to go on to reading the probfile
// if could not read a solution in the correct format returns -1
int readSolution(char* solnFile){
	int i,j, val;
	char o;
	int scanned =1;
	FILE* f = fopen(solnFile, "r");

	if(f == 0)
		return  setState(0,"Error opening solution file");
	if (!fscanf(f,"%d", &dimension)){
		setState(0,"Error reading dimension of the grid.\n");
		fclose(f);
		return -1;
	}
	dimtable = dimension+1;
	boxSize = getSqrt(dimension);
	if (boxSize==-1){
		fclose(f);
		return setState(0,"Dimension of the grid must be a perfect square.\n");
	}
	createStateArrays();
	ecc =0; // empty cell counter

	//populating table representing the completed sudoku grid
	for (i=1; ((i<dimtable) && (scanned==1));i++)
		for (j=1;((j<dimtable) && scanned) ; j++){
			scanned = fscanf(f," %d", &val);
			if (isValidValue(val)==-1)
				return setState(1, "Found an invalid value in the solution.\n");
			if (val==0) // count empty cells
				ecc++;
			else if (updateCell(i,j,val)==-1) // add cell to table, updating its row, clm, box
						return setState(1,"Grid contains duplicates.\n");
		}
	
	if (!scanned)
		return setState (1,"Could not read all values from the solution.\n");

	if (ecc>0)
	{	//create array of empty cells
		emptyCells=(EmptyCell*)malloc(sizeof(EmptyCell)*ecc);
		setState(2,"Some cells are incomplete.\n");
	}else
		setState(4,"All cells are completed.\n");
	return 1;
	
}


// reads the description of cages from problem file
// checks if all table cells are within a cage 				- INVALIDPROB
// checks if some cells are in more than in one cage 		- INVALIDPROB
// checks if solution is complete 							- INCOMPLETE
// checks if sum within the cage matches the expected one 	- INVALIDSOL
// in any of the cases above returns -1
// otherwise, if table satisfies the description of cages 
// (note! we are not checking for duplicates within rows/clms/boxes)
// returns 1
int readCageDescriptions(FILE* f){
	int i=0;
	int sumOfSizes=0;
	int ec=0; // counter for populating the emptyCells array
	int cages_described = 0;
	int r, c, val;
	Cage* newCage=(Cage*)malloc(sizeof(Cage));
	int hasEmpty=0; // if ==1 then cage contains 1+ empty cells
	int scanned = fscanf(f,"%d %d", &newCage->size, &newCage->expectedSum);
	// read the description of each cage
	while ((scanned==2) && (cages_described<nrOfCages))
	{
		if (newCage->expectedSum <= 0)
			return setState (0, "Invalid sum in one or more cages.\n");
		sumOfSizes+=newCage->size;
		newCage->actualSum =0;	
		i=0;
		while ((i<newCage->size) && (scanned==2)) {
			// read the row and clm indices for next cell in that cage
			scanned = fscanf(f," %d %d", &r, &c);
			val = table[r*dimtable+c];
			// check cell is not already within a cage
			if (notInCage(r,c)==-1)
				return setState(0,"Some cells are in more than one cage.\n");
			if (val==0){
				// add empty cell to emptyCells array
				EmptyCell empCell = makeEmptyCell(r,c,newCage);
				emptyCells[ec]=empCell;
				ec++; hasEmpty=1;
			}else 
				newCage->actualSum+=val; // update actual sum
			i++; // increment number of cell values from that cage we have read
		}
		 // set actual sum after reading all values
		if (scanned==2) { // if successfully scanned the cage
			// if no empty cells in that cage, check sum
			if ((newCage->actualSum!=newCage->expectedSum) && (hasEmpty==0)){
				return setState(1,"Wrong sum in one or more cages.\n");

			}
			cages[cages_described]=*newCage;
			cages_described++;
			hasEmpty=0;
			// start scanning new cage
			newCage=(Cage*)malloc(sizeof(Cage));
			scanned = fscanf(f,"%d %d", &newCage->size, &newCage->expectedSum);
		}
	}

	if (cages_described==nrOfCages)
	{
		if (sumOfSizes != dimension*dimension)
			return setState(0,"Not all cells were included in a cage.\n");
		if (state[4]) // all cells completed and no mistakes were found
			setState(3,"Solution is valid.\n");
		return 1;
	}else 
		return setState(0,"Not all cages were described. \n");
}


// if cell table[r][c] is already within a cage returns -1
// otherwise marks it on the cageMap and returns 1
int notInCage(int r, int c){
	if (cageMap[r*dimtable+c]==1){
		return -1;
	}
	else // if not, mark it as being in one
		cageMap[r*dimtable+c]=1;
	return 1;
}
