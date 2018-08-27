#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <vector>

#define N 255

using namespace std;

int main() {
  char linea[N];
  pid_t pid;
  int wstatus;
  int errorNo;
  while (true) {
    errorNo = 0;
    cout << "sh:>";
    cin.getline(linea,N);

	char* tChr;
	vector<char*> v;
	v.push_back((strtok(linea, " ")));
	while((tChr = (strtok(NULL, " ")))){
		v.push_back((tChr));
	}
	v.push_back(NULL);

  	char **comando = &v[0];

    if ((pid = fork()) == 0) 
      errorNo = execvp(comando[0], comando);/*execlp(comando, comando, opcion, opcion2, NULL);*/

      if (errorNo == -1)
        cout << "Comando no encontrado" << endl;
    else {
      wait(&wstatus);
    }
  }
}
