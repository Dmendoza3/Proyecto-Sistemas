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

using namespace std;

LPCTSTR buf;
HANDLE	mutex = INVALID_HANDLE_VALUE;
int message;

char text[STRING_SIZE];

void read() {
	while (true) {
		::Sleep(9000);
		cout << buf << endl;
	}
}

void write() {
	//while (true) {
		WaitForSingleObject(mutex, INFINITE);
		cout << ":> ";
		cin.getline(text, STRING_SIZE);
		CopyMemory((PVOID)buf, text, STRING_SIZE);
		_getch();
		::ReleaseMutex(mutex);
	//}
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
		"share_memory_comunication"
	);

	buf = (LPTSTR)::MapViewOfFile(shmem, FILE_MAP_ALL_ACCESS, 0, 0, STRING_SIZE);

	//Hilos

	thread writeM(write);
	thread readM(read);
	
	writeM.join();
	readM.join();

	::UnmapViewOfFile(buf);
	::CloseHandle(shmem);
	::ReleaseMutex(mutex);

	return 0;
}
