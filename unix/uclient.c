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

#include "uclient.h"

int initDir(){
    int r=mkdir("misc",0777);
    if(r!=0 && errno!=EEXIST){
        printf("Errore creazione directory di log.\n");
        return 1;
    }
    return 0;
}

int sendLSTF(){
    pthread_t pt;
    if(pthread_create(&pt,NULL,pthread_LSTF,NULL)){
        printf("Errore creazione thread LSTF.\n");
        return 1;
    }

    if(pthread_join(pt,NULL)){
        printf("Errore join thread LSTF.\n");
        return 1;
    }
    return 0;
}

int sendLSTR(){
    pthread_t pt;
    if(pthread_create(&pt,NULL,pthread_LSTR,NULL)){
        printf("Errore creazione thread LSTF.\n");
        return 1;
    }

    if(pthread_join(pt,NULL)){
        printf("Errore join thread LSTF.\n");
        return 1;
    }
    return 0;
}

int sendENCR(unsigned int seed, char *path){
    struct xorArgs val;
    val.seed=seed;
    val.path=path;

    pthread_t pt;
    if(pthread_create(&pt,NULL,pthread_ENCR,&val)){
        printf("Errore creazione thread LSTF.\n");
        return 1;
    }

    if(pthread_join(pt,NULL)){
        printf("Errore join thread LSTF.\n");
        return 1;
    }
    return 0;
}

int sendDECR(unsigned int seed, char *path){
    struct xorArgs val;
    val.seed=seed;
    val.path=path;

    pthread_t pt;
    if(pthread_create(&pt,NULL,pthread_DECR,&val)){
        printf("Errore creazione thread LSTF.\n");
        return 1;
    }

    if(pthread_join(pt,NULL)){
        printf("Errore join thread LSTF.\n");
        return 1;
    }
    return 0;
}

void *pthread_LSTF(void *arg){
    int sock, msglen;
    struct sockaddr_in serveraddr;
    char message[BUFSIZE];
    char server[16]="";
    unsigned short port=0;

    memset(server, 0, sizeof(server));
    if(loadAddress(server,&port)!=0){
        printf("Errore nella lettura del file di configurazione.\n");
    }

    printf("Connessione a %s:%u\n",server,port);

    sock=socket(AF_INET, SOCK_STREAM, 0);
    if(sock<0){
        printf("Errore apertura socket.\n");
        return NULL;
    }
    memset((char *)&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    if(inet_pton(AF_INET,server,&serveraddr.sin_addr)<=0){
        printf("Errore inet_pton.\n");
        return NULL;
    }
    serveraddr.sin_port=htons(port);

    if(connect(sock,(const struct sockaddr *)&serveraddr,sizeof(serveraddr))<0){
        printf("Errore connect socket.\n");
        return NULL;
    }

    memset(message, 0, BUFSIZE);
    sprintf(message,"LSTF");

    //invio il comando
    msglen=write(sock,message,strlen(message));
    if(msglen<0){
        printf("Errore write sul socket.\n");
        return NULL;
    }

    //attendo risposta
    bool first=true;
    while(true){
        memset(message, 0, BUFSIZE);
        msglen=read(sock,message,BUFSIZE);
        if(msglen<0){
            printf("Errore read dal socket.");
            return NULL;
        }

        if(msglen==0 || strcmp(message,"400")==0 || strcmp(message,"500")==0){
            break;
        }

        if(strcmp(message,"\r\n.\r\n")==0){
            printf("Fine lista.\n");
            break;
        }
        else if(strcmp(message,"300")!=0 && strstr(message,"\r\n")==NULL){
            if(first)
                printf("Byte\tPath\n");
            
            char bytes[15]="";
            char path[400]="";
            sscanf(message,"%s %[^\n]%*s",bytes,path);
            printf("%s\t%s\n",bytes,path);
            first=false;
        }
    }

    close(sock);

    return NULL;
}

void *pthread_LSTR(void *arg){
    int sock, msglen;
    struct sockaddr_in serveraddr;
    char message[BUFSIZE];
    char server[16]="";
    unsigned short port=0;

    memset(server, 0, sizeof(server));
    if(loadAddress(server,&port)!=0){
        printf("Errore nella lettura del file di configurazione.\n");
    }

    printf("Connessione a %s:%u\n",server,port);

    sock=socket(AF_INET, SOCK_STREAM, 0);
    if(sock<0){
        printf("Errore apertura socket.\n");
        return NULL;
    }
    memset((char *)&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    if(inet_pton(AF_INET,server,&serveraddr.sin_addr)<=0){
        printf("Errore inet_pton.\n");
        return NULL;
    }
    serveraddr.sin_port=htons(port);

    if(connect(sock,(const struct sockaddr *)&serveraddr,sizeof(serveraddr))<0){
        printf("Errore connect socket.\n");
        return NULL;
    }

    memset(message, 0, BUFSIZE);
    sprintf(message,"LSTR");

    //invio il comando
    msglen=write(sock,message,strlen(message));
    if(msglen<0){
        printf("Errore write sul socket.\n");
        return NULL;
    }

    //attendo risposta
    bool first=true;
    while(true){
        memset(message, 0, BUFSIZE);
        msglen=read(sock,message,BUFSIZE);
        if(msglen<0){
            printf("Errore read dal socket.");
            return NULL;
        }

        if(msglen==0 || strcmp(message,"400")==0 || strcmp(message,"500")==0){
            break;
        }
        
        if(strcmp(message,"\r\n.\r\n")==0){
            printf("Fine lista.\n");
            break;
        }
        else if(strcmp(message,"300")!=0 && strstr(message,"\r\n")==NULL){
            if(first)
                printf("Byte\tPath\n");
            char bytes[15]="";
            char path[400]="";
            sscanf(message,"%s %[^\n]%*s",bytes,path);
            printf("%s\t%s\n",bytes,path);
            first=false;
        }
    }

    close(sock);

    return NULL;
}

void *pthread_ENCR(void *arg){
    struct xorArgs *val=(struct xorArgs *)arg;
    unsigned int seed=val->seed;
    char *path=val->path;

    int sock, msglen;
    struct sockaddr_in serveraddr;
    char message[BUFSIZE];
    char server[16]="";
    unsigned short port=0;

    memset(server, 0, sizeof(server));
    if(loadAddress(server,&port)!=0){
        printf("Errore nella lettura del file di configurazione.\n");
    }

    printf("Connessione a %s:%u\n",server,port);

    sock=socket(AF_INET, SOCK_STREAM, 0);
    if(sock<0){
        printf("Errore apertura socket.\n");
        return NULL;
    }
    memset((char *)&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    if(inet_pton(AF_INET,server,&serveraddr.sin_addr)<=0){
        printf("Errore inet_pton.\n");
        return NULL;
    }
    serveraddr.sin_port=htons(port);

    if(connect(sock,(const struct sockaddr *)&serveraddr,sizeof(serveraddr))<0){
        printf("Errore connect socket.\n");
        return NULL;
    }

    memset(message, 0, BUFSIZE);
    sprintf(message,"ENCR %i %s",seed,path);

    msglen=write(sock,message,strlen(message));
    if(msglen<0){
        printf("Errore write sul socket.\n");
        return NULL;
    }

    //attendo la risposta
    while(true){
        memset(message, 0, BUFSIZE);
        msglen=read(sock,message,BUFSIZE);
        if(msglen<0){
            printf("Errore read dal socket.");
            return NULL;
        }

        if(strcmp(message,"200")==0){
            printf("File %s cifrato con successo.\n",path);
            if(storeSeed(path,seed)==0)
                printf("Ultimo seed per %s: %i\n",path,seed);
            break;
        }
        else if(strcmp(message,"400")==0 || strcmp(message,"500")==0)
            break;
        else{
            printf("%s\n",message);
        }
    }

    close(sock);

    return NULL;
}

void *pthread_DECR(void *arg){
    struct xorArgs *val=(struct xorArgs *)arg;
    unsigned int seed=val->seed;
    char *path=val->path;

    int sock, msglen;
    struct sockaddr_in serveraddr;
    char message[BUFSIZE];
    char server[16]="";
    unsigned short port=0;

    memset(server, 0, sizeof(server));
    if(loadAddress(server,&port)!=0){
        printf("Errore nella lettura del file di configurazione.\n");
    }

    printf("Connessione a %s:%u\n",server,port);

    sock=socket(AF_INET, SOCK_STREAM, 0);
    if(sock<0){
        printf("Errore apertura socket.\n");
        return NULL;
    }
    memset((char *)&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    if(inet_pton(AF_INET,server,&serveraddr.sin_addr)<=0){
        printf("Errore inet_pton.\n");
        return NULL;
    }
    serveraddr.sin_port=htons(port);

    if(connect(sock,(const struct sockaddr *)&serveraddr,sizeof(serveraddr))<0){
        printf("Errore connect socket.\n");
        return NULL;
    }

    memset(message, 0, BUFSIZE);
    sprintf(message,"DECR %i %s",seed,path);

    msglen=write(sock,message,strlen(message));
    if(msglen<0){
        printf("Errore write sul socket.\n");
        return NULL;
    }

    //attendo la risposta
    while(true){
        memset(message, 0, BUFSIZE);
        msglen=read(sock,message,BUFSIZE);
        if(msglen<0){
            printf("Errore read dal socket.");
            return NULL;
        }

        if(strcmp(message,"200")==0){
            printf("File decifrato con successo.\n");
            break;
        }
        else if(strcmp(message,"400")==0 || strcmp(message,"500")==0)
            break;
        else{
            printf("%s\n",message);
        }
    }

    close(sock);

    return NULL;
}
