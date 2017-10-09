/*
MIT License

Copyright (c) 2017 Emanuele Giona

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
