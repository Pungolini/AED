
#include "board_process.h"


/************************* DEFINICAO DAS ESTRUTURAS **************************/

struct Board_Place_Struct
{
    int value;          //Valor do elemento da matriz
    bool visited;       //true (1) = visitado
                        //false (0) = não visitado
};

struct _Lista_Caminho
{
    int value;                              //Valor do elemento da matriz
    short c;                                  //Coluna
    short l;                                  //Linha
    struct _Lista_Caminho *next;             //Proximo Elemento
};


struct _Board_Info
{
    short rows;           //Numero de linhas da matriz     
    short cols;           //Numero de colunas da matriz
    short c0;             //Coluna do ponto inicial
    short l0;             //Linha do ponto inicial
    int val0;           //Valor do elemento da matriz (l0,c0)
    char mode;          //Variante
    short steps;          //Numero de passos
};

Board_Info board_info;          //Variável global que contém a informação inicial (alterar por valor)

Board_Place** board;            //Onde iremos armazenar a matriz estilo (linha, coluna)

Lista_Caminho *head = NULL;     //Apontador para inicio do caminho solução

short lc[2];                      //variável global que permite guardar a diferença matriz pequena -> matriz grande
short ri, ci;                     //Variável gloval que contém, respetivamente o numero de linhas e colunas da matriz original
short n_passos = 0;               //Variável que contabiliza o numero de passos
short k_max = 0;                  //Variável que guarda o numero de passos máximo


/******** GETTER AND SETTERS **********/



/****** BOARD_INFO *********/


short getCols(void)
{
    return board_info.cols;
}


short getRows(void)
{
    return board_info.rows;
}

short getSteps(void)
{
    return board_info.steps;
}

short firstRow(void)
{
    return board_info.l0;
}

short firstCol(void)
{
    return board_info.c0;
}

char getMode(void)
{
    return board_info.mode;
}


/********* LISTA_CAMINHO **********/


Lista_Caminho *nextNode(Lista_Caminho *node)
{
    return node->next;
}
void fillNode(Lista_Caminho *node, short row, short col,int value)
{
    node->l = row;
    node->c = col;
    node->value = value;
}

int getNodeValue(Lista_Caminho *node)
{
    return node->value;
}


short getNodeRow(Lista_Caminho *node)
{
    return node->l;
}


short getNodeCol(Lista_Caminho *node)
{
    return node->c;
}




/************* BOARD_PLACE **************/


int getBoardValue(short row,short col)
{
    return board[row][col].value;
}


void markAsVisited(short row, short col)
{
    board[row][col].visited = true;
}

void markAsUnvisited(short row, short col)
{
    board[row][col].visited = false;
}

bool isVisited(short row, short col)
{

    return ((board[row][col].visited == true) ? true : false);
}


/******************************************************************************
void allocMatrix(void)
*
* Argumentos: void
*           
* Retorno:  void

*
* Descrição: Alocar memória (variável do tipo Board_Place) para a matriz
*
*****************************************************************************/

void allocMatrix(void)
{
    short   rows = getRows(),
            cols = getCols();

    board = (Board_Place**)malloc(rows*sizeof(Board_Place*));

    if (board == NULL)
        exit(0);

    for(short i = 0; i < rows; i++)
    {

        board[i] = (Board_Place*)malloc(cols*sizeof(Board_Place));

        if (board[i] == NULL)
            exit(0);
    }
}



/******************************************************************************
void free_board(void)
*
* Argumentos: void
*           
* Retorno:  void

*
* Descrição: Função que faz free do board global
*****************************************************************************/

void free_board(void)
{

    for(short i = 0; i < getRows(); i++)
        free(board[i]); 

    free(board);
    
}

/******************************************************************************
void insert (short x, short y)
* Argumentos: short x,y: linha e coluna do elemento da matriz a inserir 
* Retorno:  void
* Descrição: Função que insere na lista um nó com a informação da linha(x),
coluna(y) e valor
*****************************************************************************/


void insert(short x, short y, Lista_Caminho** _lista)
{

    char mode = getMode();
    Lista_Caminho *aux = (Lista_Caminho*)malloc(sizeof(Lista_Caminho));

    if (aux == NULL)
        exit(0);


    // Preencher node aux
    fillNode(aux,x + lc[0], y + lc[1],getBoardValue(x,y));


    if (mode == 'A' || mode == 'B' || mode == 'C' || mode == 'D')
    {

        aux->next = head;
        head = aux;
        return;
    }

    aux->next = *_lista;
    *_lista = aux;
}


/******************************************************************************
void freeList (Lista_Caminho *head_local)
*
* Argumentos: Lista_Caminho *head_local: apontador para a cabeça da lista do 
caminho
*
* Retorno:  void
*
* Descrição: Função que faz free à lista com apontador para a cabeça da mesma
*****************************************************************************/

void freeList (Lista_Caminho *head_local)
{

    Lista_Caminho *aux1;

    
    while (head_local != NULL)
    {
       aux1 = head_local;
       head_local = head_local->next;
       free(aux1);
    }
    head_local = NULL;
}

/******************************************************************************
void freeList_E_F (Lista_Caminho **head_local)
*
* Argumentos: Lista_Caminho **head_local: pointer para o endereço da 
cabeça da lista do caminho
*
* Retorno:  void
*
* Descrição: Função que faz free à lista com duplo ponteiro para a cabeça 
da mesma
*****************************************************************************/

void freeList_E_F (Lista_Caminho **head_local)
{

    Lista_Caminho *aux;
    
    while (*head_local != NULL)
    {
       aux = *head_local;
       *head_local = (*head_local)->next;
       free(aux);
    }
    *head_local = NULL;
}

/******************************************************************************
void writeNoSolution(const char *)
*
* Argumentos: const char* outputFile: ficheiro de escrita da solução
*
* Retorno:  void
*
*
* Descrição: Função que caso não exista solução, escreve tal no ficheiro, repetindo
os dados do problema e acrescentando -1 no fim

*****************************************************************************/

void writeNoSolution(const char *outputFile)
{

    FILE *fp = fopen(outputFile,"a+");

    if (fp == NULL)
        exit(0);

    {
		fprintf(fp,"%hi %hi %c %hi %d %hi %hi\n\n",ri,ci,board_info.mode,board_info.l0+lc[0],board_info.c0+lc[1],board_info.steps,-1);
		//printf("%d %d %c %d %d %d %d\n\n",ri,ci,board_info.mode,board_info.l0+lc[0],board_info.c0+lc[1],board_info.steps,-1);
	}

    fclose(fp);
}

/******************************************************************************
void write_Solution (const char *)
{
*
* Argumentos: const char* outputFile: ficheiro de escrita da solução
*
* Retorno:  void
*
* Descrição: Função que escreve solução caso esta exista
O cabeçalho desta solução irá variar de variante para variante e apresenta depois
o caminho obtido
*****************************************************************************/

void write_Solution (const char *outputFile)
{

    FILE *fp = fopen(outputFile,"a+");
    Lista_Caminho *aux;

    if (fp ==NULL)
        exit(0);
    

    aux = head;

    if(board_info.mode =='A' || board_info.mode == 'B')
    {
        
        fprintf(fp,"%d %d %c %d %d %d %d\n",ri,ci,board_info.mode,board_info.l0+lc[0],board_info.c0+lc[1],board_info.steps, board_info.steps);

        for (aux = head; aux !=NULL; aux = aux->next)
            fprintf (fp, "%d %d %d\n", aux->l, aux->c, aux->value);
            
    }
    else
    {
         if (board_info.mode =='C' || board_info.mode == 'D')
            fprintf(fp,"%d %d %c %d %d %d %d\n",ri,ci,board_info.mode,aux->l,aux->c, board_info.steps, board_info.steps);

        else if((board_info.mode =='E' || board_info.mode == 'F'))
            fprintf(fp,"%d %d %c %d %d %d %d\n",ri,ci,board_info.mode,aux->l,aux->c, board_info.steps, k_max);
              
        for (aux = aux->next; aux !=NULL; aux = aux->next)
            fprintf (fp, "%d %d %d\n", aux->l, aux->c, aux->value);
            
    }
        fputc('\n',fp);

    fclose(fp);    
}

/******************************************************************************
void Alloc_A_B(FILE* fp)
*
* Argumentos: FILE *fp: ficheiro de entrada
*
* Retorno:  void
*
* Descrição: Função para a variante A e B (uma vez que apenas estas 2 variantes irão 
alocar uma matriz de dimensões inferiores às do problema) que lê o ficheiro, 
e aloca os elementos necessários nesta matriz de dimensão inferior
*****************************************************************************/
void Alloc_A_B(FILE*fp)
{
    

    short *dim_mat,rr = 0; 
    bool flag_alloc = false;
    int teste = 0;

    //Recebe os valores limites da matriz pequena
    dim_mat = getMatrixLimits();
    allocMatrix();

    for ( short r = 0; r < ri; ++r)
		{
			for (short  c = 0, cc = -1; c < ci; ++c)
			{

				// Ignorar numeros ate chegar a coluna da submatriz
				if( (c < dim_mat[0])|| (c > dim_mat[1] ) || (r < dim_mat[2]) || (r > dim_mat[3]))
					teste = fscanf(fp, "%d", &teste);

				else
				{				
					// Guardar linha,coluna e valor na posicao da matriz
					if(fscanf(fp, "%d", &(board[rr][++cc].value)) == 1)
					{	
                        markAsUnvisited(rr,cc);
						

						if ((r == board_info.l0) && (c == board_info.c0) && (flag_alloc == false))
						{   
                            //variável global auxiliar para a  transformação matriz pequena -> matriz grande
                            lc[0]=r-rr;
                            lc[1]=c-cc;

                            //Novos valores para a matriz pequena
							board_info.val0 = board[rr][cc].value;
							board_info.c0 = cc;
							board_info.l0 = rr;
                            //Não voltar a redefinir
							flag_alloc = true;
						}
                        //Incrementar o numero de linhas da matriz pequena
						if (cc == (board_info.cols -1))
								rr++;
					}
				}
			}
		}

        free(dim_mat);

}

/******************************************************************************
void Alloc_C_D_E_F(FILE*fp)
*
* Argumentos: FILE *fp: ficheiro de entrada
*
* Retorno:  void
*
* Descrição: Para as variantes C, D, E e F iremos ler toda a matriz e utilizar
todos estes valores
*****************************************************************************/

void Alloc_C_D_E_F(FILE*fp)
{

    allocMatrix();

	for (short r = 0; r < ri; ++r)
	{
		for (short c = 0; c < ci; ++c)
		{
				if(fscanf(fp, "%d", &(board[r][c].value)) == 1)	
					markAsUnvisited(r,c);
		}
	}
}

/******************************************************************************
void getMatrixFromFile(const char* file, char *outputFile)
*
* Argumentos: const char* file: ficheiro de entrada
              char *outputFile: ficheiro de saída para escrita de solução
*
* Retorno:  void
*
* Descrição: Esta função irá ler os dados iniciais e com irá chamar funções
que farão processamento deste dados; função que deteta sempre que estamos
na presença de uma nova
*****************************************************************************/

void getMatrixFromFile(const char* file, const char *outputFile)
{   
    int teste;

	FILE *fpIn = fopen(file,"r");
	if (!fpIn)
		exit(0);

	while (fscanf(fpIn,"%hi %hi %c %hi %hi %hi",&board_info.rows, &board_info.cols, &board_info.mode, &board_info.l0,&board_info.c0, &board_info.steps) == 6)
	{   
        //guardar nlinhas e ncolunas iniciais em variável gloval
        ri = board_info.rows;
        ci = board_info.cols;

        //Ver se as condições inicias são válidas, se não for, saltar problema
		if(checkConfig() == 0)
		{   
			writeNoSolution(outputFile);

            for(short i = 0; i < board_info.rows*board_info.cols; i++)
                teste = fscanf(fpIn, "%d", &teste);          
            continue;
		}

        //Alocar matriz de acordo com o problema
        if(board_info.mode == 'A' || board_info.mode == 'B')
            Alloc_A_B(fpIn);
        
        else
            Alloc_C_D_E_F(fpIn);

        ProcessaVariante (outputFile);

        ResetParametros();
	}
    
	fclose(fpIn);
}



short recursivaE_F(short x, short y, bool (*restrictionCompare)(int x1, int x2), Lista_Caminho** lista_pai)
{      //(x,y): COORDENADA ONDE A RECURSA ESTÁ

    short coord[2] = {x, y};
    Lista_Caminho* lista_daqui = NULL;
    bool flag_new_caminho = false;

    n_passos++;
    
    markAsVisited(x,y);

    for(short dir = 0; dir < DIRECTIONS; ++dir)
    {
        getDirection(x, y, coord, dir);


        if(inBounds(board_info.rows, board_info.cols, coord[0], coord[1]) == false)
            continue;
        
            
        if(restrictionCompare(getBoardValue(x,y), getBoardValue(coord[0],coord[1])) == false)
            continue;
        
        
        if (isVisited(coord[0],coord[1]) == true)
            continue;
        

        //ENCONTROU UMA PROXIMA COORDENADA VÁLIDA
        if(recursivaE_F(coord[0], coord[1], restrictionCompare, &lista_daqui))
            flag_new_caminho = true;
        
    }

    //Está num beco sem saída, não há mais nenhuma coordenada válida

    if (n_passos > k_max)
    {
        k_max = n_passos;
        if(*lista_pai != NULL)
        {   
            //Apagar a lista do pai que contém um parte de um caminho que já não interessa
            freeList_E_F(lista_pai);
        }

        insert(x,y, lista_pai);
        flag_new_caminho = 1;
    }
    else if(flag_new_caminho == true)
    {

        if(*lista_pai != NULL)
        {      
             //Apagar a lista do pai que contém um parte de um caminho que já não interessa
            freeList_E_F(lista_pai);
        }
        insert(x,y, &lista_daqui);
        *lista_pai = lista_daqui;
    }

    n_passos--;
    markAsUnvisited(x,y);
    return flag_new_caminho;
}

//retorna 0 caso não tenha encontrado caminho 
//retorna 1 em sucesso
int main_E_F(bool (*restrictionCompare)(int x1, int x2), short l, short c)
{
    short coord[2] = {l,c};
    bool flag_new_caminho = false;
    Lista_Caminho* lista_daqui = NULL;
    short   rows = getRows(),
            cols = getCols();

    // Marcar ponto como visitado
    markAsVisited(l,c);


    for(short dir = 0; dir < DIRECTIONS; ++dir)
    {
        getDirection(l, c, coord, dir);


        if(inBounds(rows, cols, coord[0], coord[1]) == false)
            continue;
        
            
        if(restrictionCompare(getBoardValue(l,c), getBoardValue(coord[0],coord[1])) == false)
            continue;
        
        
        if (isVisited(coord[0],coord[1]) == true)
            continue;
        


        //inserido depois de teste
        if(recursivaE_F(coord[0], coord[1], restrictionCompare, &lista_daqui))
            flag_new_caminho = true;
        
    }

    if(flag_new_caminho == true)
    {
        freeList(head);
        head = lista_daqui;
        insert(l, c, &head);        
    }

    markAsUnvisited(l,c);
   
    return flag_new_caminho;
}

/******************************************************************************
void solveProblems(bool (*restrictionCompare)(int x1, int x2), char *outputFile)
*
* Argumentos: bool (*restrictionCompare)(int x1, int x2): função que irá
* determinar qual a condição a verificar (isSmallerThan ou isEven);

              char *outputFile: ficheiro de saída para escrita de solução
*
* Retorno:  void
*
* Descrição: Esta função irá percorrer todos os pontos da matriz e, de acordo
com a variante, irá chamar a função que ativará a recursiva para indicar o 
caminho solução.
*****************************************************************************/

void solveProblems(bool (*restrictionCompare)(int x1, int x2), const char *outputFile)
{

    bool flag_solucao = 0;
    char mode = getMode();
    short   Col0 = firstCol(),
            Row0 = firstRow(),
            rows = getRows(),
            cols = getCols();


    if(mode == 'A' || mode == 'B')
    {
        if(main_A_B_C_D(restrictionCompare, Row0, Col0) == 0)
        {
            writeNoSolution(outputFile);
            return;
        }
        else
        {
            write_Solution (outputFile);
            return;   
        }   
    }
    for (short l = 0; l < rows; l++)
    {
       for (short c = 0; c < cols; c++)
        {   

            if(mode == 'C' || mode == 'D')
            {
                if(main_A_B_C_D(restrictionCompare, l, c))
                {
                    write_Solution(outputFile);
                    return;
                }
            }
            else
            {
                if(main_E_F(restrictionCompare, l, c))
                    flag_solucao = true;
            
            }
        }
    }

    if (flag_solucao == false)
        writeNoSolution(outputFile);
    else
        write_Solution(outputFile);
}

/******************************************************************************
int main_A_B_C_D(bool (*restrictionCompare)(int x1, int x2), int l, int c)
*
* Argumentos: bool (*restrictionCompare)(int x1, int x2): função que irá
* determinar qual a condição a verificar (isSmallerThan ou isEven);

              int l: linha do ponto inicial;
              int c: coluna do ponto inicial;

* Retorno:  1: encontrou caminho;
            0: não encontrou caminho;
*
* Descrição: Esta função irá receber as coordenadas de um ponto. Através de 
um ciclo for irão ser analisados todos os pontos vizinhos e verificadas certas
condições (estar na matriz, elemento não visitado e condição de variante). Se
tal for verificado iremos ativar a função recursiva. Caso contrário iremos 
colocar a célula como não visitada e não teremos solução para esse ponto inicial
*****************************************************************************/

short main_A_B_C_D(bool (*restrictionCompare)(int x1, int x2), short l, short c)
{
    char  mode = getMode();
    short coord[2] = {l,c};
    short rows = getRows(),
          cols = getCols();    

    if(mode == 'A' || mode == 'B')
    {
        l = firstRow();
        c = firstCol();
    }


    markAsVisited(l,c);

    for(short dir = 0; dir < DIRECTIONS; ++dir)
    {
        getDirection(l, c, coord, dir);


        if(inBounds(rows, cols, coord[0], coord[1]) == false)
            continue;


        if(restrictionCompare(getBoardValue(l,c), getBoardValue(coord[0],coord[1])) == false)
            continue;
        
        if(isVisited(coord[0],coord[1]) == true)
            continue;

        if(backtrackDFS(coord[0], coord[1], restrictionCompare))
        {
            if((mode != 'A') && (mode !='B'))
                insert(l, c, NULL);
        
            return 1;
        }

    }

    markAsUnvisited(l,c);

    return 0;
}

/******************************************************************************
int backtrackDFS(int x, int y, bool (*restrictionCompare)(int x1, int x2))
{    
*
* Argumentos: bool (*restrictionCompare)(int x1, int x2): função que irá
* determinar qual a condição a verificar (isSmallerThan ou isEven);

              int x1: linha do ponto de entrada;
              int x2: coluna do ponto de entrada;

* Retorno:  1: encontrou caminho;
            0: não encontrou caminho;
*
* Descrição: Esta função irá receber as coordenadas de um ponto. Através de 
um ciclo for irão ser analisados todos os pontos vizinhos e verificadas certas 
condições (estar na matriz, elemento não visitado e condição de variante). Se 
tal for verificado a recursiva será novamente invocada mas agora como parâmetro
de entrada o novo vizinho que passou nas verificações anteriormente referidas.
Irá fazer este processo até chegar obter o numero de passos desejado.
*****************************************************************************/

short backtrackDFS(short x, short y, bool (*restrictionCompare)(int x1, int x2))
{    
    short coord[2] = {x, y};
    short rows = getRows(),
          cols = getCols(),
          steps = getSteps();

    n_passos++;
    if(n_passos == steps)
    {
        insert(x, y, NULL);
        return 1; // ENCONTREI CAMINHO
    }

    board[x][y].visited = 1;

    for(short dir = 0; dir < DIRECTIONS; ++dir)
    {
        getDirection(x, y, coord, dir);

        if(inBounds(rows, cols, coord[0], coord[1]) == false)
            continue;


        if(restrictionCompare(getBoardValue(x,y), getBoardValue(coord[0],coord[1])) == false)
            continue;
        
        if(isVisited(coord[0],coord[1]) == true)
            continue;

        //ENCONTROU UMA PROXIMA COORDENADA VÁLIDA
        if(backtrackDFS(coord[0], coord[1], restrictionCompare))
        {
            //Meter na list as coordenadas do board
            insert(x,y, NULL);
            return 1;   //AVISAR TODOS OS PAIS QUE FOI ENCONTRADO UM CAMINHO
        }
    }

    //Está num beco sem saída, não há mais nenhuma coordenada válida
    n_passos--;
    markAsUnvisited(x,y);
    return 0;
}