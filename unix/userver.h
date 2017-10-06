/* include guard: header comuni */
#ifndef commonH
#define commonH

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#endif
/* --- --- */

//#define __USE_POSIX
//#define __USE_BSD
//#define __USE_XOPEN_EXTENDED
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <sys/mman.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <signal.h>
#include <sys/signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define __USE_GNU
#include <omp.h>
#include <pthread.h>

#include "../common/utility.h"

#define PATHLEN 300
#define BUFSIZE 1024

#define STATE_OK "200"
#define STATE_PENDING "300"
#define STATE_ERROR "400"
#define STATE_UNAVAIL "500"

#define LOGFILE "misc/server.log"

char folder[PATHLEN];
unsigned short port;
int threadNum;

struct request{
    int ID;
    char *folder;
    char *address;
    char *message;
    int sock;
    struct request *next;
};

char lastError[600];

int nextReqID;
int numReqs;
struct request *first;
struct request *last;

pthread_mutex_t reqMutex;
pthread_cond_t reqCond;

volatile bool run;

int XOR(int a, int b);

int fileXOR(char srcfile[], char dstfile[], long dim, int seed);

int sendMessage(int sock, char message[]);

int encrypt(char src[], int seed, int sock);

int decrypt(char src[], int seed, int sock);

int listFolder(char folder[], int sock);

int listRecursive(char folder[], int sock);

int parseRequest(char folder[], char message[], int sock);

int addRequest(pthread_mutex_t *mutex,pthread_cond_t *cond,char *folder,char *address,char *message,int sock);

struct request* removeRequest(pthread_mutex_t *mutex);

void *task(void *arg);

int executeServer(char folder[], unsigned short port, int threadNum);

void showHelp(char *command);

int main(int argc, char *argv[]);

static void makeDaemon();

void sigIgnorer(int signal);

void sigHandler(int signal);
