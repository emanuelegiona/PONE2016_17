#include "wclient.h"

int initDir() {
	BOOL r = CreateDirectory("misc",NULL);
	if (r != TRUE && GetLastError() != ERROR_ALREADY_EXISTS) {
		printf("Errore creazione directory di log.\n");
		return 1;
	}
	return 0;
}

int sendLSTF() {
	HANDLE wt = CreateThread(NULL,0,wthread_LSTF,NULL,0,NULL);
	if (wt==NULL) {
		printf("Errore creazione thread LSTF.\n");
		return 1;
	}

	WaitForSingleObject(wt,INFINITE);
	CloseHandle(wt);

	return 0;
}

int sendLSTR() {
	HANDLE wt = CreateThread(NULL, 0, wthread_LSTR, NULL, 0, NULL);
	if (wt == NULL) {
		printf("Errore creazione thread LSTR.\n");
		return 1;
	}

	WaitForSingleObject(wt, INFINITE);
	CloseHandle(wt);

	return 0;
}

int sendENCR(unsigned int seed, char *path) {
	struct xorArgs val;
	val.seed = seed;
	val.path = path;

	HANDLE wt = CreateThread(NULL, 0, wthread_ENCR, &val, 0, NULL);
	if (wt == NULL) {
		printf("Errore creazione thread ENCR.\n");
		return 1;
	}

	WaitForSingleObject(wt, INFINITE);
	CloseHandle(wt);

	return 0;
}

int sendDECR(unsigned int seed, char *path) {
	struct xorArgs val;
	val.seed = seed;
	val.path = path;

	HANDLE wt = CreateThread(NULL, 0, wthread_DECR, &val, 0, NULL);
	if (wt == NULL) {
		printf("Errore creazione thread DECR.\n");
		return 1;
	}

	WaitForSingleObject(wt, INFINITE);
	CloseHandle(wt);

	return 0;
}

DWORD WINAPI wthread_LSTF(LPVOID arg) {
	WSADATA wsaData;
	SOCKET sock = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, serveraddr;
	char message[BUFSIZE];
	int res;
	int msglen = BUFSIZE;
	char strPort[6];
	char server[16] = "";
	unsigned short port = 0;

	ZeroMemory(server, sizeof(server));
	if (loadAddress(server, &port) != 0) {
		printf("Errore nella lettura del file di configurazione.\n");
	}

	printf("Connessione a %s:%u\n", server, port);
	snprintf(strPort, 6, "%u", port);

	//inizializzaizone WinSock
	res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0) {
		printf("Errore WSAStartup: %i\n", res);
		return 1;
	}

	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.ai_family = AF_INET;
	serveraddr.ai_socktype = SOCK_STREAM;
	serveraddr.ai_protocol = IPPROTO_TCP;

	//server address e porta
	res = getaddrinfo(server, strPort, &serveraddr, &result);
	if (res != 0) {
		printf("Errore getaddrinfo: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		//crea socket
		sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (sock == INVALID_SOCKET) {
			printf("Errore socket: %d\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		//connessione al server
		res = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (res == SOCKET_ERROR) {
			closesocket(sock);
			sock = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (sock == INVALID_SOCKET) {
		printf("Errore connessione al server\n");
		WSACleanup();
		return 1;
	}

	ZeroMemory(message, BUFSIZE);
	sprintf(message, "LSTF");

	//invia dati
	res = send(sock, message, strlen(message), 0);
	if (res == SOCKET_ERROR) {
		printf("Errore send: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	//chiusura connessione, non vanno inviati dati
	res = shutdown(sock, SD_SEND);
	if (res == SOCKET_ERROR) {
		printf("Errore shutdown: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	//ricezione finché server non chiude connessione
	bool first = true;
	while (true) {
		ZeroMemory(message,BUFSIZE);
		res = recv(sock, message, BUFSIZE, 0);
		if (res > 0) {
			if (strcmp(message, "400") == 0 || strcmp(message, "500") == 0)
				break;

			if (strcmp(message, "\r\n.\r\n") == 0) {
				printf("Fine lista.\n");
				break;
			}
			else if (strcmp(message, "300") != 0 && strstr(message, "\r\n") == NULL) {
				if (first)
					printf("Byte\tPath\n");

				char bytes[15] = "";
				char path[400] = "";
				sscanf(message, "%s %[^\n]%*s", bytes, path);
				printf("%s\t%s\n", bytes, path);
				first = false;
			}
		}
	}

	//cleanup
	closesocket(sock);
	WSACleanup();

	return 0;
}

DWORD WINAPI wthread_LSTR(LPVOID arg) {
	WSADATA wsaData;
	SOCKET sock = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, serveraddr;
	char message[BUFSIZE];
	int res;
	int msglen = BUFSIZE;
	char strPort[6];
	char server[16] = "";
	unsigned short port = 0;

	ZeroMemory(server, sizeof(server));
	if (loadAddress(server, &port) != 0) {
		printf("Errore nella lettura del file di configurazione.\n");
	}

	printf("Connessione a %s:%u\n", server, port);
	snprintf(strPort, 6, "%u", port);

	//inizializzaizone WinSock
	res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0) {
		printf("Errore WSAStartup: %i\n", res);
		return 1;
	}

	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.ai_family = AF_INET;
	serveraddr.ai_socktype = SOCK_STREAM;
	serveraddr.ai_protocol = IPPROTO_TCP;

	//server address e porta
	res = getaddrinfo(server, strPort, &serveraddr, &result);
	if (res != 0) {
		printf("Errore getaddrinfo: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		//crea socket
		sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (sock == INVALID_SOCKET) {
			printf("Errore socket: %d\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		//connessione al server
		res = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (res == SOCKET_ERROR) {
			closesocket(sock);
			sock = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (sock == INVALID_SOCKET) {
		printf("Errore connessione al server\n");
		WSACleanup();
		return 1;
	}

	ZeroMemory(message, BUFSIZE);
	sprintf(message, "LSTR");

	//invia dati
	res = send(sock, message, strlen(message), 0);
	if (res == SOCKET_ERROR) {
		printf("Errore send: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	//chiusura connessione, non vanno inviati dati
	res = shutdown(sock, SD_SEND);
	if (res == SOCKET_ERROR) {
		printf("Errore shutdown: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	//ricezione finché server non chiude connessione
	bool first = true;
	while (true) {
		ZeroMemory(message,BUFSIZE);
		res = recv(sock, message, BUFSIZE, 0);
		if (res > 0) {
			if (strcmp(message, "400") == 0 || strcmp(message, "500") == 0)
				break;

			if (strcmp(message, "\r\n.\r\n") == 0) {
				printf("Fine lista.\n");
				break;
			}
			else if (strcmp(message, "300") != 0 && strstr(message, "\r\n") == NULL) {
				if (first)
					printf("Byte\tPath\n");

				char bytes[15] = "";
				char path[400] = "";
				sscanf(message, "%s %[^\n]%*s", bytes, path);
				printf("%s\t%s\n", bytes, path);
				first = false;
			}
		}
	}

	//cleanup
	closesocket(sock);
	WSACleanup();

	return 0;
}

DWORD WINAPI wthread_ENCR(LPVOID arg) {
	struct xorArgs *val = (struct xorArgs *)arg;
	unsigned int seed = val->seed;
	char *path = val->path;

	WSADATA wsaData;
	SOCKET sock = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, serveraddr;
	char message[BUFSIZE];
	int res;
	int msglen = BUFSIZE;
	char strPort[6];
	char server[16] = "";
	unsigned short port = 0;

	ZeroMemory(server, sizeof(server));
	if (loadAddress(server, &port) != 0) {
		printf("Errore nella lettura del file di configurazione.\n");
	}

	printf("Connessione a %s:%u\n", server, port);
	snprintf(strPort, 6, "%u", port);

	//inizializzaizone WinSock
	res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0) {
		printf("Errore WSAStartup: %i\n", res);
		return 1;
	}

	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.ai_family = AF_INET;
	serveraddr.ai_socktype = SOCK_STREAM;
	serveraddr.ai_protocol = IPPROTO_TCP;

	//server address e porta
	res = getaddrinfo(server, strPort, &serveraddr, &result);
	if (res != 0) {
		printf("Errore getaddrinfo: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		//crea socket
		sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (sock == INVALID_SOCKET) {
			printf("Errore socket: %d\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		//connessione al server
		res = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (res == SOCKET_ERROR) {
			closesocket(sock);
			sock = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (sock == INVALID_SOCKET) {
		printf("Errore connessione al server\n");
		WSACleanup();
		return 1;
	}

	ZeroMemory(message, BUFSIZE);
	sprintf(message, "ENCR %i %s", seed, path);

	//invia dati
	res = send(sock, message, strlen(message), 0);
	if (res == SOCKET_ERROR) {
		printf("Errore send: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	//chiusura connessione, non vanno inviati dati
	res = shutdown(sock, SD_SEND);
	if (res == SOCKET_ERROR) {
		printf("Errore shutdown: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	//ricezione finché server non chiude connessione
	bool first = true;
	while (true) {
		res = recv(sock, message, BUFSIZE, 0);
		if (res > 0) {

			if (strcmp(message, "200") == 0) {
				printf("File %s cifrato con successo.\n", path);
				if (storeSeed(path, seed) == 0)
					printf("Ultimo seed per %s: %i\n", path, seed);
				break;
			}
			else if (strcmp(message, "400") == 0 || strcmp(message, "500") == 0) {
				break;
			}
			else {
				printf("%s\n", message);
			}
		
}
		else if (res == 0) {
			//fine recv
			//break;
		}
	}

	//cleanup
	closesocket(sock);
	WSACleanup();

	return 0;
}

DWORD WINAPI wthread_DECR(LPVOID arg) {
	struct xorArgs *val = (struct xorArgs *)arg;
	unsigned int seed = val->seed;
	char *path = val->path;

	WSADATA wsaData;
	SOCKET sock = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, serveraddr;
	char message[BUFSIZE];
	int res;
	int msglen = BUFSIZE;
	char strPort[6];
	char server[16] = "";
	unsigned short port = 0;

	ZeroMemory(server, sizeof(server));
	if (loadAddress(server, &port) != 0) {
		printf("Errore nella lettura del file di configurazione.\n");
	}

	printf("Connessione a %s:%u\n", server, port);
	snprintf(strPort, 6, "%u", port);

	//inizializzaizone WinSock
	res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0) {
		printf("Errore WSAStartup: %i\n", res);
		return 1;
	}

	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.ai_family = AF_INET;
	serveraddr.ai_socktype = SOCK_STREAM;
	serveraddr.ai_protocol = IPPROTO_TCP;

	//server address e porta
	res = getaddrinfo(server, strPort, &serveraddr, &result);
	if (res != 0) {
		printf("Errore getaddrinfo: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		//crea socket
		sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (sock == INVALID_SOCKET) {
			printf("Errore socket: %d\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		//connessione al server
		res = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (res == SOCKET_ERROR) {
			closesocket(sock);
			sock = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (sock == INVALID_SOCKET) {
		printf("Errore connessione al server\n");
		WSACleanup();
		return 1;
	}

	ZeroMemory(message, BUFSIZE);
	sprintf(message, "DECR %i %s", seed, path);

	//invia dati
	res = send(sock, message, strlen(message), 0);
	if (res == SOCKET_ERROR) {
		printf("Errore send: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	//chiusura connessione, non vanno inviati dati
	res = shutdown(sock, SD_SEND);
	if (res == SOCKET_ERROR) {
		printf("Errore shutdown: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	//ricezione finché server non chiude connessione
	bool first = true;
	while (true) {
		res = recv(sock, message, BUFSIZE, 0);
		if (res > 0) {
			if (strcmp(message, "200") == 0) {
				printf("File decifrato con successo.\n");
				break;
			}
			else if (strcmp(message, "400") == 0 || strcmp(message, "500") == 0) {
				break;
			}
			else {
				printf("%s\n", message);
			}
		}
		else if (res == 0) {
			//fine recv
			//break;
		}
	}

	//cleanup
	closesocket(sock);
	WSACleanup();

	return 0;
}
