#include <iostream>
#include <thread>
#include <stdio.h>
#include <SDKDDKVer.h>
#include <Windows.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <tchar.h>


#define STRING_SIZE 1024

struct Segmento {
	bool servidorFlag;
	bool clienteFlag;
	char mensajeS[STRING_SIZE];
	char mensajeC[STRING_SIZE];
};

Segmento *segmento;

using namespace std;

LPCTSTR bufS;
HANDLE shmemS = INVALID_HANDLE_VALUE;
/*LPCTSTR bufC;
HANDLE	mutex1 = INVALID_HANDLE_VALUE;
HANDLE	mutex2 = INVALID_HANDLE_VALUE;*/
int message;

char text[STRING_SIZE];

void read() {
	while (true) {
		//WaitForSingleObject(mutex1, INFINITE);
		//cout << bufC << endl;
		//::ReleaseMutex(mutex1);
		//segmento->clienteFlag = true;
		segmento = (Segmento *)MapViewOfFile(shmemS, // handle to map object
			FILE_MAP_ALL_ACCESS,  // read/write permission
			0,
			0,
			STRING_SIZE);
		segmento->clienteFlag = true;
		while (segmento->clienteFlag);
		cout << segmento->mensajeC << endl;
	}
}

void write() {
	while (true) {
		cout << "Cliente" << endl;
		//WaitForSingleObject(mutex2, INFINITE);
		cout << ":> " << endl;
		cin.getline(text, STRING_SIZE);
		strcpy_s(segmento->mensajeS, text);
		segmento->servidorFlag = false;
		CopyMemory((PVOID)bufS, segmento, (sizeof(Segmento)));
		_getch();
		//::ReleaseMutex(mutex2);
	}
}

int main() {

	//Compartiendo memoria
	//HANDLE shmemC = INVALID_HANDLE_VALUE;

	//mutex1 = ::CreateMutex(NULL, FALSE, "mutex_comunication1");
	//mutex2 = ::CreateMutex(NULL, FALSE, "mutex_comunication2");

	shmemS = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		STRING_SIZE,
		"share_memory_comunicationS"
	);

	/*shmemC = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		STRING_SIZE,
		"share_memory_comunicationC"
	);*/

	bufS = (LPTSTR)::MapViewOfFile(shmemS, FILE_MAP_ALL_ACCESS, 0, 0, STRING_SIZE);
	//bufC = (LPTSTR)::MapViewOfFile(shmemC, FILE_MAP_ALL_ACCESS, 0, 0, STRING_SIZE);*/

	//Hilos

	thread writeM(write);
	thread readM(read);

	writeM.join();
	readM.join();

	//Server
	::UnmapViewOfFile(bufS);
	::CloseHandle(shmemS);
	//Client
	//::UnmapViewOfFile(bufC);
	//::CloseHandle(shmemC);
	//::ReleaseMutex(mutex1);
	//::ReleaseMutex(mutex2);

	return 0;
}
