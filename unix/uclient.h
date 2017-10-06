/* include guard: header comuni */
#ifndef commonH
#define commonH

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#endif
/* --- --- */

/* include guard: unix client */
#ifndef uclientH
#define uclientH

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/stat.h>
#include <errno.h>

#include "../common/utility.h"

#define BUFSIZE 1024

struct xorArgs{
    unsigned int seed;
    char *path;
};

int initDir();
int sendLSTF();
int sendLSTR();
int sendENCR(unsigned int seed, char *path);
int sendDECR(unsigned int seed, char *path);

void *pthread_LSTF(void *arg);
void *pthread_LSTR(void *arg);
void *pthread_ENCR(void *arg);
void *pthread_DECR(void *arg);

#endif
/* --- --- */
