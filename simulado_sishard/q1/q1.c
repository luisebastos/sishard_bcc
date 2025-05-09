// Leia o PDF ou MD antes de iniciar este exercício!

// Faça os includes necessários aqui
// #include ...
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

// Struct para passar os semáforos para as threads
typedef struct {
    sem_t *sA;
    sem_t *sB;
    sem_t *sC;
    sem_t *sD;
} rdv_args;

void *thread1(void *_arg) {
    rdv_args *args = _arg;

    printf("A\n");

    // Libera B, C e D
    sem_post(args->sA);
    sem_post(args->sA);
    sem_post(args->sA);

    // Espera B, C e D sinalizarem que terminaram
    sem_wait(args->sB);
    sem_wait(args->sC);
    sem_wait(args->sD);

    printf("E\n");

    return NULL;
}

void *thread2(void *_arg) {
    rdv_args *args = _arg;

    sem_wait(args->sA); // espera A
    printf("B\n");
    sem_post(args->sB); // sinaliza que B terminou

    return NULL;
}

void *thread3(void *_arg) {
    rdv_args *args = _arg;

    sem_wait(args->sA); // espera A
    printf("C\n");
    sem_post(args->sC); // sinaliza que C terminou

    return NULL;
}

void *thread4(void *_arg) {
    rdv_args *args = _arg;

    sem_wait(args->sA); // espera A
    printf("D\n");
    sem_post(args->sD); // sinaliza que D terminou

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t threads[4];

    // Inicializa semáforos
    sem_t sA, sB, sC, sD;
    sem_init(&sA, 0, 0);
    sem_init(&sB, 0, 0);
    sem_init(&sC, 0, 0);
    sem_init(&sD, 0, 0);

    rdv_args args = {
        .sA = &sA,
        .sB = &sB,
        .sC = &sC,
        .sD = &sD
    };

    void *(*funcoes[])(void *) = {thread1, thread2, thread3, thread4};

    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, funcoes[i], &args);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&sA);
    sem_destroy(&sB);
    sem_destroy(&sC);
    sem_destroy(&sD);

    return 0;
}
