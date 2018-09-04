#include <iostream>
#include <stdio.h>
#include <string.h>
#include <typeinfo>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>

#define N 10

  using namespace std;


int main(){
	int x = 0;
	int ppid = getpid();
	int pid = -1;
	int status;
	cout << "Parent is " << ppid <<endl;
//	pid = fork();
	for (int i = 0, pid = fork(); i < N && pid == 0; i++){
		if(i != N - 1){
			if((pid = fork()) == 0){
				cout << "\t\t\tThis is a new child ppid=" << getppid() << endl;
			}else{
				if(i == 0)
					cout << "First" << endl;
				cout << "\t\t\t\tForked. pid="<< getpid() << endl;
				wait(&status);
				//break;
			}
		}else{
			cout << "\t\t\t\t\t\tLast child." << endl;
		}
	}
	if(getpid() == ppid){
		wait(&status);
		cout << "Once pid=" << ppid << endl;
	} else {
		cout << "\t\tExiting fork pid=" << getpid << " ,ppid=" << getppid() << endl;
		return 0;
	}


	return 0;
}
