#include "board_process.h"

int main(int argc, const char **argv)
{
	char *outputFile = NULL;

	outputFile = checkArgs(argc, argv);
	
    getMatrixFromFile(argv[1], outputFile);

	free(outputFile);
	
	return 0;
}