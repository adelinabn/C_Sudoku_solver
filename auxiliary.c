# include "auxiliary.h"
# include <math.h>

// ########################################################################################## //


int state[5] = {0};
char* msg[5]; 
int dimension; 
int dimtable;  
int boxSize;   
int* table;	   
int* cageMap;  
Cage* cages;  
Linear* rows;	  
Linear* clms;
Linear* boxes;	  
EmptyCell* emptyCells; 
int ecc; 
int nrOfCages;			  


// ########################################################################################## //

// displays array of cages
void displayArrCages(){
	for (int i=0 ; i< nrOfCages; i++){
		printf("cage nr:\t%d\n",i );
		printf("___________________\n");
		printf("size:\t\t%d\nexpected sum:\t%d\nactual sum:\t%d\n",cages[i].size, cages[i].expectedSum,cages[i].actualSum );
		printf("\n\n");
	}
}

// initialises arrays of states and messages
void initialiseState(){
	for (int i=0;i<5;i++){
		state[i]=0;
		msg[i]="";
	}
	
}

// returns -1 if value is within acceptable range for current problem
int isValidValue(int i){
	if (i>=0 && i<dimtable)
		return 1;
	else 
		return -1;
}


// sets all array cells of array arr to 0
void initialise_2dArray(int* arr, int size){
	for (int i=0; i<size;i++)
		for (int j=0; j<size; j++)
			arr[i*size+j]=0;			
}


// changes the state and the associated message
// if state =solved or state = all cells completed returns 1, otherwise returns -1
int setState (int state_index , char* m){
	state[state_index]=1;
	msg[state_index] = m;
	if (state_index==3 || state_index==4)
		return 1;
	else return -1;
}


// creates the arrays used to retrieve the state of the sudoku at any time
// while the empty cells are being filled
void createStateArrays()
{
	table = (int*)(malloc(sizeof(int)*dimtable*dimtable));
	cageMap= (int*)(malloc(sizeof(int)*dimtable*dimtable));
	initialise_2dArray(cageMap,dimtable);
	rows=(Linear*)malloc(sizeof(Linear)*dimtable);
	clms=(Linear*)malloc(sizeof(Linear)*dimtable);
	boxes=(Linear*)malloc(sizeof(Linear)*boxSize*boxSize);
	// actually we will only use boxes [0][0] to [2][2] but for simplicity of addressing
	// we make it boxsize and not boxsize-1
	for (int i=1; i<dimtable;i++)
	{
		rows[i].values = (int*)malloc(sizeof(int)*dimtable);
		clms[i].values = (int*)malloc(sizeof(int)*dimtable);
		
	}
	for (int i=0; i<boxSize; i++)
		for (int j=0; j<boxSize;j++)
	{	
		boxes[i*boxSize+j].values=(int*)malloc(sizeof(int)*dimtable);
		for (int k=1;k<dimtable;k++)
			boxes[i*boxSize+j].values[k]=0;
	}
}


// returns square root as int or -1 if n is not a perfect square, or is 0
int getSqrt(int n)
{
	if (n<=0)
		return -1;
	int temp = sqrt(n);
	if (temp*temp==n)
		return temp;
	else
		return -1;
}


// will display the solution represented by the table[][] array
void displayArrTable()
{
	int i,j;
	for (i=1; i<dimtable ; i++){
		for (j=1; j<dimtable ; j++)
			printf("%d  ",table[i*dimtable+j]);
		printf("\n");
	}
}

// reurns an empty cell to be added to the emptyCells[] array
EmptyCell makeEmptyCell(int r, int c, Cage* ptr_cage)
{
	EmptyCell e;
	e.row = rows[r].values;
	e.clm = clms[c].values;
	e.box = boxes[(r-1)/boxSize*boxSize+(c-1)/boxSize].values;
	e.pCage = ptr_cage;
	e.iTable= r*dimtable+c;
	return e;
}


// overwrites file fname with current values from table[][]
int writeSolution(char* fname)
{
	FILE* f= fopen(fname, "w");
	if(f==0)
		return -1;
	fprintf(f, "%d\n", dimension);
	for (int i=1; i<dimtable; i++)
	{
		for (int j=1; j<dimtable; j++)
			fprintf(f, "%d ",table[i*dimtable+j] );
		fprintf(f, "\n");
	}
	fclose(f);
	return 1;	
}


// displays the states applicable to the current sudoku problem & solution
void displayStates()
{
	char* state_names[4] = {"Invalid Problem", "Invalid Solution","Incomplete solution", "Solved"}; 
	printf("State of the sudoku:\n");
	for (int i= 0 ; i<4; i++)
		if (state[i]){
				printf("%s : \t", state_names[i]);
				printf("%s\n",msg[i]);
		}
}


// concatenates str1 with str2 returning a pointer to joint string
char* concat(const char* str1, const char* str2)
{
    char* result;
    asprintf(&result, "%s%s", str1, str2);
    return result;
}
