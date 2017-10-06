# PONE2016_17

## Multiplatform client/server file cypher application (C, POSIX API, WIN32 API)

Il progetto consiste nella realizzazione di un sistema multipiattaforma di cifratura e decifratura da remoto sotto forma di client e server, installati rispettivamente nel computer “di controllo” e nel computer “vittima”.
L’obiettivo iniziale era strutturare il progetto in modo tale da produrre un’unica coppia di file client.c e server.c, che, basandosi sull’identificazione del sistema operativo in fase di linking, invocasse funzioni definite con lo stesso nome in header files specifici al sistema operativo riconosciuto, per imitare l’idea di interfaccia del paradigma object-oriented.
Questa struttura è stata realizzata limitatamente al codice del client, siccome il requisito di attivare il server come un daemon in ambiente Unix/Linux avrebbe già portato alla creazione di un file dal quale iniziare l’esecuzione diverso dal file compilato dal codice in server.c.

This project consists in a multiplatform client/server file cipher application, where the client is installed in the "master" system and the server in the "slave" system.

### Notes

- Theoretically, there is no maximum supported file size; however, tests have run flawlessly with files up to 10 GB for the Unix/Linux implementation and up to 4 GB for the Windows implementation, omitting the remaining tests up to 10 GB, assuming there would be no problem whatsoever

- OpenMP is used during the cipher/decipher phase, while any other threaded operation makes use of native threads (POSIX or WIN32)

- Unix/Linux implementation runs as a daemon

### Known bugs

- Client files visualization fails, printing out quite a lot spaces instead of actual file paths; this happens when client and server applications run from different computers. This doesn't happen when both client and server applications run within the same computer.
