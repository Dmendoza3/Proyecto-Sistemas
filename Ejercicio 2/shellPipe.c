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
  char linea[N];
  pid_t pid;
  int wstatus;
  int errorNo;
  bool active = true;

  while (active) {
    errorNo = 0;

    cout << "sh:>";
    cin.getline(linea,N);

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
	
	/*for(int i = 0; i < v.size(); i++){
		for(int j = 0; j < v[i]->size() - 1; j++){
			cout << (*(v[i]))[j] << ", ";
		}
		cout << endl;
	}*/

	//TODO: Pipe from here

	if(v.size() > 1){
		int p[2];
		pipe(p);

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
  }
  return 0;
}
