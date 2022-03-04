#include "utils.h"

               



/******************************************************************************
bool is SmallerThan (int, int)
*
* Argumentos: short x1: valor de um elemento da matriz
             short x2: valor de um elemento da matriz
*           
* Retorno:  true: se x2 > x1
            false: se x2 <= x1

*
* Descrição: Compara dois numeros 
*
*****************************************************************************/

bool isSmallerThan(int x1, int x2)
{
	return  (x2 > x1 ? true : false);
}


/******************************************************************************
bool isEven (in, int)
*
* Argumentos: short x1: valor de um elemento da matriz
              short x2: valor de um elemento da matriz
*           
* Retorno:  true: se x2 for par
            false: se x2 for ímpar

*
* Descrição: Analisa se x2 é par ou ímpar
*
*****************************************************************************/

bool isEven(int x1, int x2)
{
	return  (x2 % 2 == 0 ? true : false);
}
/******************************************************************************
bool fileExists (const char*)
*
* Arguments: filename - nome do ficheiro
*           
* Returns:  1 - ficheiro existe
            0 - não existe

*
* Descrição: verifica se o ficheiro de entrada existe
*
*****************************************************************************/
bool fileExists(const char * filename)
{
    
    FILE *file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
        return true;
    }
    return false;
}

/******************************************************************************
short isValidPuz (const char*)
*
* Argumentos: cons char* filename: nome do ficheiro de entrada
*           
* Retorno:  1: se a extensão for .puz
            0: caso contrário

*
* Descrição: Analisa se a extensão do ficheiro de entrada é ".puz"
*
*****************************************************************************/
bool isValidPuz(const char *filename)
{
	bool res = 0;
    short size = 0;

	if ((size = strlen(filename)) > 4)
	{
		  res = filename[size - 1] == 'z' &&
				filename[size - 2] == 'u' &&
				filename[size - 3] == 'p' &&
				filename[size - 4] == '.';
	}

	return res;

}

/******************************************************************************
char *checkArgs (int, char **, char*)
*
* Argumentos: int argc: numero de argumentos de entrada
              char **argv: argumentos de entrada
              char *exitfile: ficheiro de saída
*           
* Retorno:  exitFile: ficheiro de saída

*
* Descrição: Verifica se o ficheiro de entrada inserido é válido invocando as funções
fileExists e isVAlidPuz. Aloca memória para o ficheiro de saída e escreve o devido nome
(isto é, retira o .puz e insere .path)
*
*****************************************************************************/

char *checkArgs(int argc, const char **argv)
{
	bool flag = (argc >= 2) && fileExists(argv[1]) && isValidPuz(argv[1]);

	if (flag == false)
		exit(0);


	char *exitFile = (char*)calloc((strlen(argv[1]) + 2),sizeof(char));
	if (!exitFile) exit(0);

    // Criar nome do ficheiro de saida: nome.puz <-> nome.path
	strncpy(exitFile,argv[1],(strlen(argv[1]) - strlen("puz")));
	strcat(exitFile,"path");

	/* Apagar conteudo de algum ficheiro de teste anterior */
	FILE *fp = fopen(exitFile,"w");
	fclose(fp);

	return exitFile;
}

/******************************************************************************
short checkConfig ()
*
* Argumentos: void
*           
* Retorno:  1: se os argumentos da matriz forem válidos;
            0: caso contrário

*
* Descrição: Analisando a primeira linha da matriz, processa a informação vendo se o 
ponto inicial está dentro da matriz (variantes A e B), a variante é uma das opções
(isto é A, B, C, D, E ou F) e se o numero de passos é valido (positivo e inferior ao 
numero de elementos da matriz no caso das variantes A,B,C e D)
*
*****************************************************************************/

//Função que confirma se os parâmteros de entrada para cada variante são válidos ou não
bool checkConfig(void)
{
	short flagVariante = 1;
    char    var = getMode();
    short   steps = getSteps(),
            rows = getRows(),
            cols = getCols();

	switch (var)
	{
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			break;
		default:
			flagVariante = false;
	}

	bool flagPosInit = inBounds(rows,cols,firstRow(),firstCol());

	bool flagSteps = ((steps < rows * cols) && (steps > 0));


	if( var == 'E' || var == 'F')
    {
		flagSteps = true;
        flagPosInit = true;
    }

	else if( var == 'C' || var == 'D')
    {
		flagPosInit = true;
    }

    if (cols <= 0 || rows <= 0)
    {
        flagPosInit = false;
    }

	return flagVariante && flagPosInit && flagSteps;
}

/*******************************************************************************************
int* getMatrixLimits()
*
* Argumentos: void
*           
* Retorno:  int *vect_dim: vetor de 4 posições com os valores dos 4 pontos limite da matriz

*
* Descrição: Caso a variante seja A ou B alocámos memória para uma matriz mais pequena
que possua k passos (sempre que possível) em torno de todas as direções do ponto inicial
Esta função obtem assim os pontos limite (cima esquerdo), (cima direita), (baixo esquerda),
e (baixo direita) retornando-os.
*
********************************************************************************************/

short* getMatrixLimits(void)
{   
    short *vect_dim;
    short   Col0 = firstCol(),
            Row0 = firstRow(),
            rows = getRows(),
            cols = getCols(),
            steps = getSteps();

    vect_dim = (short*)malloc(4*sizeof(&vect_dim));

    vect_dim[0] =  Col0 - steps;
	vect_dim[1] =  Col0 + steps;

	vect_dim[2] =  Row0 - steps;
	vect_dim[3] =  Row0 + steps;

	if (vect_dim[0] < 0)
		vect_dim[0] = 0;
	
	if (vect_dim[1] > cols - 1)
		vect_dim[1] = cols - 1;


	if (vect_dim[2] < 0)
		vect_dim[2] = 0;
	
	if (vect_dim[3] > rows - 1)
		vect_dim[3] = rows - 1;
    
    //Atualizar o numero de linhas e colunas da matriz pequena
    rows = vect_dim[3] - vect_dim[2] + 1;
    cols = vect_dim[1] - vect_dim[0] + 1;

    return vect_dim;
}



/******************************************************************************
void getDirection(int, int, int *, int)
{
*
* Argumentos: short x e short y: coordenada linha, coluna
              int coord[2]: vetor que irá atualizar as coordenada de um ponto 
vizinho de (x,y)
*             short dir: flag que irá variar o ponto vizinho
* Retorno:  void

*
* Descrição: Esta função, de acordo com o valor de dir , irá obter numa dada 
direção um ponto vizinho de (x, y) e restaurá-lo em coord[];

*****************************************************************************/

//Função para obter as coordenadas do novo ponto
void getDirection(short x, short y, short coord[2], short dir)
{
    coord[0] = x;
    coord[1] = y;

    switch (dir)
    {
    case 0: //direção para cima
        coord[0]--;
        break;

    case 1: //direção diagonal direita cima
        coord[0]--;
        coord[1]++;
        break;

    case 2: //direção para direita
        coord[1]++;
        break;

    case 3: //direção diagonal direita baixo
        coord[0]++;
        coord[1]++;
        break;

    case 4: //direção para baixo
        coord[0]++; 
        break;

    case 5: //direção diagonal esquerda baixo
        coord[0]++; 
        coord[1]--;
        break;
    
    case 6: //direção esquerda
        coord[1]--; 
        break;

    case 7: //direção diagonal esquerda cima
        coord[0]--;
        coord[1]--; 
        break;
    }
}
/******************************************************************************
void ProcessaVariante (char *outputFile)
*
* Argumentos: char *outputFile: ficheiro de saída
*
* Retorno:  void
*
* Descrição: Função que analisa a variante e identifica o problema a resolver, 
isto é, encontrar as células de forma crescente ou encontrar as células pares.


*****************************************************************************/
void ProcessaVariante (const char *outputFile)
{
    char mode = getMode();

    if (mode == 'A' || mode == 'C' || mode == 'E')
        solveProblems(isSmallerThan, outputFile);

    else
        solveProblems(isEven, outputFile);    
    
}

/******************************************************************************
void ResetParametros(void)
*
* Argumentos: void
*
* Retorno:  void
*
* Descrição: Depois de processar toda a informação para uma matriz e apresentar
a solução é necessário reiniciar as variáveis para o caso de existir mais do que 
uma matriz num ficheiro
*****************************************************************************/

void ResetParametros(void)
{

    freeList(head);
    free_board();
    head = NULL;
    k_max = 0; 
    n_passos = 0;
    ri = 0;
    ci = 0;
    lc[0] = 0;
    lc[1] = 0;
}
