// implemente aqui!
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s ./path/para/algum/programa\n", argv[0]);
        return 1;
    }
    while (1) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Erro no fork");
            exit(1);
        }
        if (pid == 0) {
            execvp(argv[1], &argv[1]);
            perror("Erro no execvp");
            exit(1);
        }
        printf("starting %s with pid=%d\n", argv[1], pid);
        fflush(stdout); 
        int status;
        waitpid(pid, &status, 0);
    }
    return 0;
}