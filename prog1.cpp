#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;

int main(int argc, char* argv[]){
	long val1 = strtol(argv[1], NULL, 10);
	val1 *= 5;

	cout << val1;
	return 0;
}
