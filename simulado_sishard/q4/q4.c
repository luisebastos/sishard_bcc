// Leia o enunciado antes de iniciar este exercício!

// inclua libs aqui!
#include <stdio.h>      // printf, fprintf
#include <stdlib.h>     // malloc, free, atoi, exit, rand, srand
#include <unistd.h>     // read, close, sleep
#include <fcntl.h>      // open, O_RDONLY
#include <time.h>       // time
#include <pthread.h>    // pthread_t, pthread_create, mutex, cond
#include <string.h>     // strlen (se precisar, opcional)

typedef struct
{
    char *palavra; // palavra a ser classificada
    int res;       // retorno da thread virá aqui!
} t_arg;

#define MAX_LINE_LENGTH 256
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int threads_ativas = 0;
int limite_threads;

/*
 * Função que simula a classificação de uma palavra
 */
int classifier(char *palavra)
{
    int sleep_time = rand() % 1 + 4;

    // Espera por uma qtde aleatória de segundos
    sleep(sleep_time);

    int soma = 157;

    for (int i = 0; palavra[i] != '\0'; i++)
    {
        soma += palavra[i];
    }

    int res = (soma % 3);

    return res;
}

/*
 * Classificação em Threads!
 */
void *classifier_thread(void *_arg)
{
    t_arg *arg = _arg;
    pthread_mutex_lock(&mutex);
    while (threads_ativas >= limite_threads) {
        pthread_cond_wait(&cond, &mutex);
    }
    threads_ativas++;
    pthread_mutex_unlock(&mutex);

    arg->res = classifier(arg->palavra);

    pthread_mutex_lock(&mutex);
    threads_ativas--;
    pthread_cond_signal(&cond);  // Acorda uma thread esperando
    pthread_mutex_unlock(&mutex);
}

/* Função que lê uma palavra de um arquivo.
 * Considere que cada linha do arquivo contem apenas uma palavra.
 *
 * Entradas:
 *   int fd: descritor do arquivo
 *
 * Saída:
 *   Ponteiro (char *) para uma string com a palavra lida (sem \n). Caso o arquivo termine, retorne NULL.
 */
char *read_line(int fd)
{
    // Seu código aqui!
    char buf[1];
    char *string = malloc(MAX_LINE_LENGTH);
    int i = 0;
    ssize_t n;
    while ((n = read(fd, buf, 1)) > 0) {
        if(buf[0] == '\n'){break;}
        string[i] = buf[0];
        i++;
    }
    string[i] = '\0';
    // close(fd);
    if (n == 0 && i == 0) {
        free(string);
        return NULL;
    }

    return string;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("USAGE:\n./q4 <n_threads> <file_name>\n");
        return 0;
    }

    int n_threads = atoi(argv[1]); // Info da linha de comando
    char *file_name = argv[2];     // Info da linha de comando

    limite_threads = atoi(argv[1]);
    
    int fd1 = open(file_name, O_RDONLY);

    if (fd1 == -1)
    {
        fprintf(stderr, "Falha ao abrir o arquivo!\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    int threads_limit = 2; // Vamos começar alocando espaço para 02 threads
    pthread_t *tids = malloc(sizeof(pthread_t) * threads_limit);
    t_arg **args = malloc(sizeof(t_arg *) * threads_limit);

    int threads_created = 0; // Quantas threads criei de fato

    for (int i = 0;; i++)
    {
        // Tenta ler uma palavra
        char *palavra = read_line(fd1);

        // Para quando não conseguir mais ler
        if (palavra == NULL || palavra[0] == '\0')
        {   
            free(palavra);
            break;
        }

        t_arg *arg = malloc(sizeof(t_arg));
        // Passa a palavra para a thread
        arg->palavra = palavra;

        // Realoca se necessário
        if (threads_limit == threads_created)
        {
            threads_limit *= 2;
            tids = realloc(tids, sizeof(pthread_t) * threads_limit);
            args = realloc(args, sizeof(t_arg *) * threads_limit);
        }

        args[i] = arg;

        pthread_create(&tids[i], NULL, classifier_thread, arg);
        printf("CRIOU THREAD %02d PARA PROCESSAR A PALAVRA %s\n", i, arg->palavra);

        threads_created++;
    }

    // Complete a main!
    int classe0 = 0, classe1 = 0, classe2 = 0;

    // Espera todas as threads terminarem e conta os resultados
    for (int i = 0; i < threads_created; i++) {
        pthread_join(tids[i], NULL);
        printf("THREAD %02d TERMINOU COM O RESULTADO %d\n", i, args[i]->res);

        if (args[i]->res == 0) classe0++;
        else if (args[i]->res == 1) classe1++;
        else if (args[i]->res == 2) classe2++;
    }

    // Imprime resultado final
    printf("RESULTADO:\n");
    printf("TOTAL CLASSE 0: %d\n", classe0);
    printf("TOTAL CLASSE 1: %d\n", classe1);
    printf("TOTAL CLASSE 2: %d\n", classe2);
    return 0;
}