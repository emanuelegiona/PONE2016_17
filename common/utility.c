#include "utility.h"

int loadAddress(char address[], unsigned short* port){
    FILE* conf=fopen("misc/client.conf","r");
    if(conf==NULL){
        strcpy(address,"127.0.0.1");
        *port=8888;
        return 1;
    }

    char line[400];
    do{
        fgets(line,400,conf);
        char *start=NULL;
        start=strstr(line,"=");
        start++;
        
        if(strstr(line,"address")!=NULL){
            int end=strlen(start)-1;
            strncpy(address,start,end);
            address[end]='\0';
        }
        else if(strstr(line,"port")!=NULL){
            *port=(unsigned short)atoi(start);
        }
    }while(line!=NULL && !feof(conf));

    /* defaults */
    if(strcmp(address,"\0")==0)
        strcpy(address,"127.0.0.1");
    if(*port<1024 || *port>65535)
        *port=8888;
    /* --- --- */

    fclose(conf);
    return 0;
}

int loadConfig(unsigned short* port, char folder[], int* threadNum){
    FILE* conf=fopen("misc/server.conf","r");
    if(conf==NULL){
        *port=8888;
        *threadNum=1;
        return 1;
    }

    char line[400];
    do{
        fgets(line,400,conf);
        char *start=NULL;
        start=strstr(line,"=");
        start++;
        
        if(strstr(line,"port")!=NULL){
            *port=(unsigned short)atoi(start);
        }
        else if(strstr(line,"folder")!=NULL){
            int end=strlen(start)-1;
            strncpy(folder,start,end);
            folder[end]='\0';
        }
        else if(strstr(line,"threadNumber")!=NULL){
            *threadNum=atoi(start);
        }
    }while(line!=NULL && !feof(conf));

    /* defaults */
    if(*port<1024 || *port>65535)
        *port=8888;
    if(*threadNum<1)
        *threadNum=1;
    /* --- --- */

    fclose(conf);
    return 0;
}

int writeLog(char filename[], char message[]){
    FILE* logFile=fopen(filename, "a+");
    if(logFile==NULL){
        char str[100];
        sprintf(str,"[I/O Error]: apertura di %s in scrittura non riuscita\n",filename);
        writeLog("misc/error.log",str);
        return 1;
    }
    fputs(message,logFile);
    fputs("\n",logFile);

    fclose(logFile);
    return 0;
}

int readLog(char filename[], FILE** logFile){
    *logFile=fopen(filename,"r");
    if(*logFile==NULL){
        char str[100];
        sprintf(str,"[I/O Error]: apertura di %s in lettura non riuscita\n",filename);
        writeLog("misc/error.log",str);
        return 1;
    }

    return 0;
}

int storeSeed(char filename[], int seed){
    char seedStr[50]="";
    sprintf(seedStr,"%i",seed);

    char str[350]="";
    strncpy(str,filename,strlen(filename));
    strncat(str,";",2);
    strncat(str,seedStr,48);

    return writeLog("misc/seeds.log",str);
}

//restituisce in *seed l'ultimo seed associato al filename[] nel file seeds.log
int loadSeed(char filename[], int* seed){
    FILE* seedLog=NULL;
    if(readLog("misc/seeds.log",&seedLog)!=0)
        return 1;

    char line[350];
    bool found=false;
    do{
        fgets(line,350,seedLog);
        char *start=NULL;
        start=strstr(line,";");
        start++;

        if(strstr(line,filename)!=NULL){
            *seed=atoi(start);
            found=true;
        }
    }while(line!=NULL && !feof(seedLog));

    fclose(seedLog);

    if(!found)
        return -1;

    return 0;
}
