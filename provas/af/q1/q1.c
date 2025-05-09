// Leia o README antes de iniciar este exercício!

// inclua as bibliotecas necessárias
// #include ...
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

typedef struct {
    sem_t *sA;
    sem_t *sB;
    sem_t *sC;
    sem_t *sD;
    sem_t *sE;
    sem_t *sF; 
    sem_t *sG; 
} rdv_args;

void *thread1(void *_arg)
{
    rdv_args *args = _arg;

    printf("A\n");
    sem_post(args->sA); 
    sem_post(args->sA); 
    sem_post(args->sA); 
    sem_post(args->sA); 

    sem_wait(args->sD); 
    sem_wait(args->sA); 
    sem_wait(args->sD); 

    printf("B\n");
    sem_post(args->sB); 
    sem_post(args->sB); 
    sem_post(args->sB); 
    sem_post(args->sB);

    sem_wait(args->sE);
    sem_wait(args->sF);
    sem_wait(args->sB);
    sem_wait(args->sE);
    sem_wait(args->sF);

    printf("C\n");
    sem_post(args->sC); 
    sem_wait(args->sG); 

    return NULL;
}

void *thread2(void *_arg)
{
    rdv_args *args = _arg;

    printf("D\n");
    sem_post(args->sD); 
    sem_post(args->sD); 
    sem_post(args->sD); 
    sem_post(args->sD); 

    sem_wait(args->sA); 
    sem_wait(args->sA); 
    sem_wait(args->sD); 

    printf("E\n");
    sem_post(args->sE); 
    sem_post(args->sE); 
    sem_post(args->sE); 
    sem_post(args->sE); 

    sem_wait(args->sB); 
    sem_wait(args->sF);

    return NULL;
}

void *thread3(void *_arg)
{
    rdv_args *args = _arg;

    sem_wait(args->sD);
    sem_wait(args->sA);

    printf("F\n");
    sem_post(args->sF); 
    sem_post(args->sF); 
    sem_post(args->sF); 
    sem_post(args->sF); 

    sem_wait(args->sE);
    sem_wait(args->sB); 

    return NULL;
}

void *thread4(void *_arg)
{
    rdv_args *args = _arg;

    sem_wait(args->sE);
    sem_wait(args->sB);
    sem_wait(args->sF);

    printf("G\n");

    sem_post(args->sG);
    sem_post(args->sC);

    return NULL;
}

int main(int argc, char *argv[])
{
    // Crie TODAS as threads. Voce deve utilizar semaforos para sincronizacao.
    pthread_t threads[4];

    // Inicializa semáforos
    sem_t sA, sB, sC, sD, sE, sF, sG;
    sem_init(&sA, 0, 0);
    sem_init(&sB, 0, 0);
    sem_init(&sC, 0, 0);
    sem_init(&sD, 0, 0);
    sem_init(&sE, 0, 0);
    sem_init(&sF, 0, 0);
    sem_init(&sG, 0, 0);

    rdv_args args = {
        .sA = &sA,
        .sB = &sB,
        .sC = &sC,
        .sD = &sD,
        .sE = &sE,
        .sF = &sF,
        .sG = &sG
    };

    // Espere por TODAS as threads
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
    sem_destroy(&sE);
    sem_destroy(&sF);
    sem_destroy(&sG);

    return 0;
}
