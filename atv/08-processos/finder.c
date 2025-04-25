#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int x, n, k;

    if (scanf("%d", &x) != 1) return 1;
    if (scanf("%d", &n) != 1) return 1;

    int* A = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &A[i]) != 1) return 1;
    }

    if (scanf("%d", &k) != 1) return 1;

    int r = n / k;
    int found = 0;

    for (int i = 0; i < k; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            sleep(5);

            int start = i * r;
            int end = start + r;

            for (int j = start; j < end; j++) {
                if (A[j] == x) {
                    exit(j); 
                }
            }
            exit(-1); 
        }
    }

    for (int i = 0; i < k; i++) {
        int status;
        pid_t pid = wait(&status);

        if (WIFEXITED(status)) {
            int result = WEXITSTATUS(status);
            if (result != 255 && !found) {  // 255 == -1
                found = 1;
                printf("Elemento %d encontrado pelo processo %d no indice %d!\n", x, i, result);
            }
        }
    }

    if (!found) {
        printf("Elemento não encontrado!\n");
    }

    free(A);
    return 0;
}
