#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h> 

#define rowIsValid(numRows, row) ((row  >= 0) && ( row  < numRows))
#define colIsValid(numCols, col) ((col  >= 0) && ( col  < numCols))
#define inBounds(numRows,numCols,x, y) (rowIsValid(numRows,x) && colIsValid(numCols,y) == true ? true : false)


extern short lc[2];                      
extern short ri, ci;                    
extern short n_passos;               
extern short k_max;   

#include "board_process.h"


short* getMatrixLimits(void);

bool checkConfig(void);
bool isEven(int,int);
bool fileExists(const char *);
bool isValidPuz(const char *);
bool isSmallerThan(int,int);


char *checkArgs(int, const char **);

void printMat(void);
void ResetParametros(void);
void freeList (Lista_Caminho *);
void freeList_E_F (Lista_Caminho **);
void ProcessaVariante (const char *);
void getDirection(short, short, short [2], short);



#endif