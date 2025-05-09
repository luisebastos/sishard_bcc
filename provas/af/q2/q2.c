// Leia o HTML ou MD antes de iniciar este exercício!

// inclua libs aqui!
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    // SUA RESPOSTA AQUI

    int ret;
    int wst;

    if(argc != 4 || atol(argv[3]) <= 0){
        printf("Argumentos errados ou valor negativo\n");
        return 1;
    }

    pid_t pai, filho;
    do {        
        filho = fork();

        if(filho == 0){

            char *args[] = {"./digitaliza", argv[1], argv[2], argv[3], NULL};
            execvp("./digitaliza", args);
            exit(1);

        } else{

            waitpid(filho, &wst, 0);
            ret = WEXITSTATUS(wst);

            if(ret >=1 && ret <=4){

                printf("Falha!\n");

            }else if(ret == 0){

                printf("digitaliza TERMINOU COM SINAL");

            }
        }

    } while (ret > 0); 

    return 0;
}