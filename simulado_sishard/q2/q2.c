// Leia o PDF ou MD antes de iniciar este exercício!

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    int ret;
    int wst;

    if(argc != 4 || atol(argv[3]) <= 0){
        printf("Argumentos errados ou valor negativo\n");
        return 1;
    }

    pid_t pai, filho;
    do {
        // Construa a chamada do executavel siscoin. Veja enunciado.
        
        // No pai, espere e trate o resultado
        
        // DICA: ao atribuir o return do siscoin, faça casting para (char)
        
        filho = fork();

        if(filho == 0){

            char *args[] = {"./siscoin", argv[1], argv[2], argv[3], NULL};
            execvp("./siscoin", args);
            exit(1);

        } else{

            waitpid(filho, &wst, 0);
            ret = WEXITSTATUS(wst);

            if(ret >=1 && ret <=4){

                printf("Falha na rede!\n");

            }else if(ret == 0){

                printf("Transferencia realizada!\n");

            }
            // printf("Status de siscoin: %d\n", ret);
        }

    } while (ret > 0); // Repita enquanto houver falha. 
                       // Para se tem problema com o valor ou a qtde de parâmetros
                       // Esta validação está sendo feita em siscoin

    return 0;
}