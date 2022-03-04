#ifndef _BOARD_PROCESS_H
#define _BOARD_PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h> 

#define DIRECTIONS 8
// Estrutura para descrever uma celula da matriz
typedef struct Board_Place_Struct Board_Place;

//Lista em que cada nó contém os dados do elemento da matriz
typedef struct _Lista_Caminho  Lista_Caminho;

// Estrutura que contem os parametros de um problema
typedef struct _Board_Info Board_Info;


extern Board_Info board_info;
extern Board_Place** board;
extern Lista_Caminho *head;

#include "utils.h"

/************* FUNCOES DE ABSTRACAO DE DADOS ****************/

/****** BOARD_INFO *********/
short getCols(void);
short getRows(void);
short getSteps(void);
short firstRow(void);
short firstCol(void);
char getMode(void);





/********** LISTA_CAMINHO *********/

short getNodeCol(Lista_Caminho *);
short getNodeRow(Lista_Caminho *);
int getNodeValue(Lista_Caminho *);

void insert (short , short , Lista_Caminho **);
void fillNode(Lista_Caminho *, short , short ,int );

/***************************************/


Lista_Caminho *initLista(short );
Lista_Caminho *nextNode(Lista_Caminho *);
bool isVisited(short , short );

/************* BOARD_PLACE **************/

bool isVisited(short , short );
int getBoardValue(short ,short );

void markAsVisited(short , short );
void markAsUnvisited(short , short );



void free_board(void);
void allocMatrix(void);
void Alloc_A_B(FILE* );
void Alloc_C_D_E_F(FILE*);
void write_Solution (const char *);
void writeNoSolution(const char *);
void getMatrixFromFile(const char*, const char *);



void solveProblems(bool (*restrictionCompare)(int, int), const char *);

short backtrackDFS(short, short , bool (*restrictionCompare)(int, int));
short recursivaE_F(short , short , bool (*restrictionCompare)(int, int), Lista_Caminho **);      //(x,y): COORDENADA ONDE A RECURSA ESTÁ

short main_A_B_C_D(bool (*restrictionCompare)(int, int), short, short);
int main_E_F(bool (*restrictionCompare)(int, int), short, short);

#endif