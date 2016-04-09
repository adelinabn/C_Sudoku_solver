
extern int dimension; 		// size of the side of sudoku square
extern int dimtable;  		// dimension +1 
extern int boxSize;	 		// size of the side of the boxes in the sudoku
extern int nrOfCages; 		// number of cages in current sudoku
extern int ecc;				// empty cell counter 


extern int state[5];   
extern char* msg[5];  // messages / descriptions corresponding to each state[]

// if any of these state[n] == 1 then 
// the corresponding state characteristic msg[n] is applicable:

// 0 -  invalid problem	 :  some cells in more than one cage / some cells not within a cage
// 1 -  invalid solution :  completely filled cage = wrong sum
//							duplicates in rows clms boxes
//							invalid value in at least one cell
// 2 -  incomplete		 :  some cells not filled in: equal to 0
// 3 -  solved			 :  all cells satisfy cage descriptions and no duplicates found in
//							any of the columns,rows or boxes
// 4 -  complete 		 :  all cells of the grid are complete (possibly not correctly)

// ###########################################################################################################################

// these are STATE ARRAYS - we can infer the state of the sudoku at any point of execution
// by checking their values

extern int* table;	    			 // 2d array for storing the solution of the sudoku
extern int* cageMap;   				 // 2d array mapping table cells to cages such that:
			   						 // cagemap[i][j]==1 if cell at table[i][j] is within a cage
extern struct Cage* cages;    		 // storing the cage descriptions
extern struct Linear* rows;       	 // storing the state of each row, such that:
			                  		 // if rows[i].values[x]==1 then value x is found within row i of the grid
extern struct Linear* clms;   		 // similar to rows;
extern struct Linear* boxes;     	 // similar to rows and clms, first box is indexed boxes[0][0]
extern struct EmptyCell* emptyCells; // array of empty cells, will traverse it while solving the sudoku

// ###########################################################################################################################


typedef struct Cage 
		{
			int size;    // nr of cells in that cage
			int expectedSum;
			int actualSum;
		} Cage;


typedef struct EmptyCell
		{
			int* row;    // pointer to linear structure storing the state of cell's row
			int* clm;    // same as for row
			int* box;    // same as for row
			Cage* pCage; // pointer to cell's cage, allowing us to update the actual sum
						 // as we are trying different possible values when solving the puzzle
			int iTable;  // index of that cell in the table[][] array
		} EmptyCell;


// linear structure for storing state of a row, box or column
typedef struct Linear
		{
			int* values;
		} Linear;






		