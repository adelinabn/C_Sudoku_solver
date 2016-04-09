#include <stdio.h>
#include "g_variables.h"
#include "check.h"

int 		setState(int state_index, char* msg);
void 		initialise_2dArray(int* arr, int size);
void 		displayArrTable();
void 		displayArrCages();  // method used when debugging to check that
								// cage descriptions are read correctly
int 		getSqrt(int n);
void		createStateArrays();
EmptyCell 	makeEmptyCell(int r, int c, Cage* ptr_cage);
int 		writeSolution(char* fname);
void 		displayStates();
int 		isValidValue(int i);
char* 		concat(const char* str1, const char* str2);
void 		initialiseState();
