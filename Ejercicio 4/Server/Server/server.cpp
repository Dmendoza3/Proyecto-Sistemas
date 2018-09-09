#include <iostream>
#include <thread>
#include <stdio.h>
#include <SDKDDKVer.h>
#include <Windows.h>
#include <string.h>
#include <stdlib.h>

#define STRING_SIZE 1024

using namespace std;
/*
struct Segmento {
	bool serverFlag;
	bool clientFlag;
	char mensajeS[STRING_SIZE];
	char mensajeC[STRING_SIZE];
};

Segmento *segmento;
*/

char *buf;
HANDLE	mutex = INVALID_HANDLE_VALUE;
int message;

void read() {
	while (true) {
	}
}

void write() {
	while (true) {
		WaitForSingleObject(mutex, INFINITE);
		cout << ">: ";
		cin
		memset(buf, , STRING_SIZE);
	}
}

int main() {

	//Compartiendo memoria

	HANDLE	shmem = INVALID_HANDLE_VALUE;

	mutex = ::CreateMutex(NULL, FALSE, "mutex_comunication");

	shmem = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		STRING_SIZE,
		"shared_memory_name"
	);

	buf = (char*)::MapViewOfFile(shmem, FILE_MAP_ALL_ACCESS, 0, 0, STRING_SIZE);

	//Hilos
	


	::UnmapViewOfFile(buf);
	::CloseHandle(shmem);
	::ReleaseMutex(mutex);

	return 0;
}
