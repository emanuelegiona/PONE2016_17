/* include guard: header comuni */
#ifndef commonH
#define commonH

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#endif
/* --- --- */

/* include guard: utility */
#ifndef utilityH
#define utilityH

int loadAddress(char address[], unsigned short* port);

int loadConfig(unsigned short* port, char folder[], int* threadNum);

int writeLog(char filename[], char message[]);

int readLog(char filename[], FILE** logFile);

int storeSeed(char filename[], int seed);

int loadSeed(char filename[], int* seed);

#endif
/* --- --- */
