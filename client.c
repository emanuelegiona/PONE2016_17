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

/* gestione OS */
#ifdef _WIN32
   #include "win/wclient.h" //custom win32 header
#else
   #include "unix/uclient.h" //custom unix header
#endif
/* --- --- */

#include "common/utility.h"

#define MAXLEN 300

void showHelp(char *command){
    printf("client~ ");
    if(strcmp(command,"HELP")!=0)
        printf("Comando non valido: %s\n\t",(command));

    printf("Usage: {comando} [seed] [path]\n\t\
Valori per seed e path sono obbligatori se il comando e' ENCR o DECR; non hanno alcun effetto sugli altri comandi.\n\n\t\
Comandi:\n\t\
LSTF\t richiede la lista dei soli file presenti nella cartella corrente\n\t\
LSTR\t richiede la lista di tutti i file presenti nella cartella corrente e sottocartelle\n\t\
ENCR\t cifra con XOR il file path utilizzando seed; seed deve essere un intero senza segno\n\t\
DECR\t decifra con XOR il file path utilizzando seed; seed deve essere un intero senza segno\n\t\
SHOW\t mostra l'ultimo seed associato ad un path dato in input\n\t\
QUIT\t termina il programma (stesso effetto del comando q)\n\t\
HELP\t mostra questo messaggio\n\n\t\
L'indirizzo e la porta del server al quale connettersi puo' essere specificato nel file misc/client.conf.\n");

    return;
}

void showHelpCL(char *command){
    printf("client~ ");
    if(strcmp(command,"-h")!=0)
        printf("Comando non valido.\n\t");

    printf("Usage: {comando} [seed] [path]\n\t\
Valori per seed e path sono obbligatori se il comando e' ENCR o DECR; non hanno alcun effetto sugli altri comandi.\n\n\t\
Comandi:\n\t\
-l\t richiede la lista dei soli file presenti nella cartella corrente\n\t\
-R\t richiede la lista di tutti i file presenti nella cartella corrente e sottocartelle\n\t\
-e\t cifra con XOR il file path utilizzando seed; seed deve essere un intero senza segno\n\t\
-d\t decifra con XOR il file path utilizzando seed; seed deve essere un intero senza segno\n\t\
-s\t mostra l'ultimo seed associato ad un path dato in input\n\t\
-h\t mostra questo messaggio\n\n\t\
L'indirizzo e la porta del server al quale connettersi puo' essere specificato nel file misc/client.conf.\n");

    return;
}

int main(int argc, char *argv[]){
    if(initDir()!=0)
        return 1;

    //shell interattiva
    if(argc==1){
        char input[MAXLEN];
        char command[MAXLEN];
        bool quit=false;
        do{
			memset(input,0,MAXLEN);
			memset(command, 0, MAXLEN);

            printf("client~ ");
            fgets(input,MAXLEN,stdin);
            sscanf(input,"%[^\n]%*s",command);

            if(strstr(command,"LSTF")!=NULL){
                sendLSTF();
            }
            else if(strstr(command,"LSTR")!=NULL){
                sendLSTR();
            }
            else if(strstr(command,"ENCR")!=NULL){
                char s[4];
                unsigned int seed=-1;
                char path[MAXLEN]="errore";

                sscanf(command,"%s %u %[^\n]%*s",s,&seed,path);
                if(seed!=-1 && strcmp(path,"errore")!=0){
                    sendENCR(seed,path);
                }
                else
                    showHelp(command);
            }
            else if(strstr(command,"DECR")!=NULL){
                char s[4];
                unsigned int seed=-1;
                char path[MAXLEN]="errore";

                sscanf(command,"%s %u %[^\n]%*s",s,&seed,path);
                if(seed!=-1 && strcmp(path,"errore")!=0){
                    sendDECR(seed,path);
                }
                else
                    showHelp(command);
            }
            else if(strstr(command,"SHOW")!=NULL){
                char s[4];
                char path[MAXLEN]="errore";

                sscanf(command,"%s %[^\n]%*s",s,path);
                if(strcmp(path,"errore")!=0){
                    int seed=-1;
                    if(loadSeed(path,&seed)>0){
                        printf("Errore nella lettura del file seeds.log\n");
                        continue;
                    }
    
                    if(seed!=-1)
                        printf("Ultimo seed associato al file %s: %i\n",path,seed);
                    else
                        printf("Il file %s non ha un seed associato.\n",path);
                }
                else
                    showHelp(command);
            }
            else if(strcmp(command,"HELP")==0 || (strcmp(command,"QUIT")!=0 && strcmp(command,"q")!=0)){
                showHelp(command);
            }
            else
                quit=true;
        }while(!quit);
    }
    //gestione opzioni da riga di comando
    else if(argc>1 && argc<5){
        if(strstr(argv[1],"-l")!=NULL){
            sendLSTF();
        }
        else if(strstr(argv[1],"-R")!=NULL){
            sendLSTR();
        }
        else if(strstr(argv[1],"-e")!=NULL && argc==4){
            unsigned int seed=-1;
            char path[MAXLEN]="errore";

            sscanf(argv[2],"%u",&seed);
            sscanf(argv[3],"%[^\n]%*s",path);
            if(seed!=-1 && strcmp(path,"errore")!=0){
                sendENCR(seed,path);
            }
            else
                showHelpCL(argv[0]);
        }
        else if(strstr(argv[1],"-d")!=NULL && argc == 4){
            unsigned int seed=-1;
            char path[MAXLEN]="errore";

            sscanf(argv[2],"%u",&seed);
            sscanf(argv[3],"%[^\n]%*s",path);
            if(seed!=-1 && strcmp(path,"errore")!=0){
                sendDECR(seed,path);
            }
            else
                showHelpCL(argv[0]);
        }
        else if(strstr(argv[1],"-s")!=NULL && argc == 3){
            char path[MAXLEN]="errore";

            sscanf(argv[2],"%[^\n]%*s",path);
            if(strcmp(path,"errore")!=0){
                int seed=-1;
                if(loadSeed(path,&seed)>0){
                    printf("Errore nella lettura del file seeds.log\n");
                    return 1;
                }

                if(seed!=-1)
                    printf("Ultimo seed associato al file %s: %i\n",path,seed);
                else
                    printf("Il file %s non ha un seed associato.\n",path);
            }
            else
                showHelpCL(argv[0]);
        }
        else{
            showHelpCL(((strcmp(argv[1],"-h")==0)?"-h":argv[0]));
        }
    }
    //comando totalmente sbagliato
    else
        showHelpCL("-h");

    return 0;
}
