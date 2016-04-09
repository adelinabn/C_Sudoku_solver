# include "g_variables.h"
int fillEmpty (int index);

// recursive method for filling in the table[][] array
// using the index of an empty cell in the emptyCells[][] array
// returns 1 if all empty cells following the current cell were successfully completed
// returns -1 if given the current value of the empty cell, the following empty cells could
// not be completed
int fillEmpty (int index)
{
	if (index>=ecc) // no more empty cells to be filled in
		return 1;
	else 
	{	
		//printf("filled: %d out of %d\n\n", index+1,ecc);
		// if there are still empty cells to be filled in
		EmptyCell empc = emptyCells[index]; // empty cell at that index
		Cage* c = empc.pCage;       		// cage of that empty cell
		int delta=c->expectedSum - c->actualSum;
		if (delta>dimension)
			delta=dimension;

		if(delta==0)
			return -1; // expecting more values in this cage, but the expected sum is already satisfied
		for (int i=1; i<delta+1 ;i++) // only try values that fit within the sum difference gap
		{ 		
			// if cell's row column and box don't already contain that value
			if ((empc.row[i]==0)&&(empc.clm[i]==0) && (empc.box[i]==0))
			{	// pretend we found a suitable value for that cell
				table[empc.iTable] = i;
				c->actualSum+=i;
				empc.row[i]=1;
				empc.clm[i]=1;
				empc.box[i]=1;
				// check if other empty cells can be filled in given this value of the current cell
				if (fillEmpty(index+1)==1)
					return 1; // completed the remaining part of the sudoku successfully

				// if no solution with current value of the cell
				// undo changes and try next possbile value
				table[empc.iTable]=0;
				c->actualSum-=i;
				empc.row[i]=0;
				empc.clm[i]=0;
				empc.box[i]=0;
			}				
		}
		return -1; // if all values failed for this cell, backtrack
				   // control passes to the recursive call from filling the previous empty cell
	} 
}
