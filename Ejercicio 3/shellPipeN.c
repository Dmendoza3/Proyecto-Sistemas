#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <termios.h>
#include <stdlib.h>
#include <fcntl.h>
#include <vector>

#define INPUT_END 1
#define OUTPUT_END 0

#define N 255

using namespace std;

int main() {
  char linea[N] = {0};
  pid_t pid;
  int wstatus;
  int errorNo;
  bool active = true;

  while (active) {
    errorNo = 0;
	linea[0] = 'a';
	
    cout << "sh:>";
    cin.getline(linea,N);

	if(linea[0] == 0){
		continue;
	}
	char* tChr;
	vector<vector<char*>* > v;
	v.push_back(new vector<char*>());
	v.back()->push_back((strtok(linea, " ")));
	while((tChr = (strtok(NULL, " ")))){
		if(strcmp(tChr, "|") == 0){
			v.back()->push_back(NULL);
			v.push_back(new vector<char*>());
			continue;
		}
		v.back()->push_back((tChr));
	}
	v.back()->push_back(NULL);
	
	//TODO: Pipe from here

	if(v.size() > 1){
		int nP = v.size();
		int p[nP - 1][2];

		for(int i = 0; i < nP; i++)
			pipe(p[i]);

		int ppid = getpid();
		int pid = -1;
		for(int i = 0, pid = fork(); i < nP && pid == 0; i++){
			if(i != nP - 1){
				if((pid = fork()) == 0){
					continue;
				}else{
					if(i != 0){
						//To recieve
						close(p[i - 1][INPUT_END]);
						dup2(p[i - 1][OUTPUT_END], STDIN_FILENO);
						close(p[i - 1][OUTPUT_END]);
					}
					
					//To send
					close(p[i][OUTPUT_END]);
					dup2(p[i][INPUT_END], STDOUT_FILENO);
					close(p[i][INPUT_END]);

					char **comando = &((*v[i])[0]);

					errorNo = execvp(comando[0], comando);
					if (errorNo == -1)
						cout << "Comando no encontrado" << endl;	  
					wait(&wstatus);
					break;
				}
			}else{
				//To recieve
				close(p[i - 1][INPUT_END]);
				dup2(p[i - 1][OUTPUT_END], STDIN_FILENO);
				close(p[i - 1][OUTPUT_END]);

				char **comando = &((*v[i])[0]);
				
				errorNo = execvp(comando[0], comando);
				if (errorNo == -1)
					cout << "Comando no encontrado" << endl;	  
				wait(&wstatus);
				break;
			}

			if(ppid == getppid())
				wait(&wstatus);
			else
				cout << "Ending.." << endl;
		}


		/*int nP = v.size() - 1;
		int p[v.size() - 1][2];
		int pipePid;
		for(int i = 0; i < nP; i++){
			pipe(p[i]);
			if((pipePid = fork()) == 0){
				
			}else{
				
			}

		}*/
		


		/*if((pid = fork()) == 0){
			if((pid = fork()) == 0){
				char **comando = &((*v[1])[0]);

				close(p[INPUT_END]);
				dup2(p[OUTPUT_END], STDIN_FILENO);
				close(p[OUTPUT_END]);

				errorNo = execvp(comando[0], comando);
				if (errorNo == -1)
					cout << "Comando no encontrado" << endl;	  
			}else{
				if((pid = fork()) == 0){
					char **comando = &((*v[0])[0]);

					close(p[OUTPUT_END]);
					dup2(p[INPUT_END], STDOUT_FILENO);
					close(p[INPUT_END]);

					errorNo = execvp(comando[0], comando);
					if (errorNo == -1)
						cout << "Comando no encontrado" << endl;	  
				}else{
					wait(&wstatus);
				}
				close(p[OUTPUT_END]);
				close(p[INPUT_END]);
			}
		}else{
			wait(&wstatus);
		}*/
	}else{
		char **comando = &((*v[0])[0]);

		if(strcmp(comando[0], "exit") == 0){
			active = false;
		} else {
			if ((pid = fork()) == 0) {
				errorNo = execvp(comando[0], comando);
				if (errorNo == -1)
					cout << "Comando no encontrado" << endl;	  
			} else {
				wait(&wstatus);
			}
		}
		
	}

	//Executed
	for(int i = 0; i < v.size(); i++)
		delete v[i];
	v.clear();
  }
  return 0;
}
