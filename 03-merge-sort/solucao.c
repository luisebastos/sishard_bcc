#include "sort-merge.h"
// ... aqui começa sua solucao.
// para compilar use
// gcc -g -Og -Wall sort.o solucao.c -o sort -pthread

// Fonte: ultilizei ia para me ajudar 

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX_ITEM 30  

void bubble_sort(char **lista, int total_itens) {
    for (int i = 0; i < total_itens - 1; i++) {
        for (int j = 0; j < total_itens - i - 1; j++) {
            if (strcmp(lista[j], lista[j + 1]) > 0) {
                char *temporario = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = temporario;
            }
        }
    }
}

void *sort(void *argumentos) {
    struct sort_args *parametros = (struct sort_args *)argumentos;

    int indice_thread = parametros->idxThread;
    int total_threads = parametros->nThreads;
    int total_itens_arquivo = parametros->lineFiles;
    FILE *arquivo_entrada = parametros->fp;
    pthread_mutex_t *mutex_arquivo = parametros->mutex_file;

    int itens_por_thread = total_itens_arquivo / total_threads;

    char **itens_lidos = malloc(itens_por_thread * sizeof(char *));
    for (int i = 0; i < itens_por_thread; i++) {
        itens_lidos[i] = malloc(TAM_MAX_ITEM);
    }

    pthread_mutex_lock(mutex_arquivo);
    for (int i = 0; i < itens_por_thread; i++) {
        char *linha = fgets(itens_lidos[i], TAM_MAX_ITEM, arquivo_entrada);

        if (linha == NULL) {
            fprintf(stderr, "Erro ao ler item na thread %d\n", indice_thread);
            itens_lidos[i][0] = '\0'; 
        } else {
            itens_lidos[i][strcspn(itens_lidos[i], "\r\n")] = '\0';  
        }
    }
    pthread_mutex_unlock(mutex_arquivo);
    bubble_sort(itens_lidos, itens_por_thread);
    char nome_arquivo_saida[16];
    sprintf(nome_arquivo_saida, "%d.txt", indice_thread);
    FILE *arquivo_saida = fopen(nome_arquivo_saida, "w");

    if (arquivo_saida == NULL) {
        perror("Erro ao criar arquivo de saída");
        pthread_exit(NULL);
    }

    for (int i = 0; i < itens_por_thread; i++) {
        fprintf(arquivo_saida, "%s\n", itens_lidos[i]);
        free(itens_lidos[i]);
    }

    free(itens_lidos);
    rewind(arquivo_saida);
    FILE *arquivo_retorno = fopen(nome_arquivo_saida, "r");
    if (arquivo_retorno == NULL) {
        perror("Erro ao reabrir arquivo de saída para retorno");
        pthread_exit(NULL);
    }
    pthread_exit(arquivo_retorno);
}

void *merge(void *argumentos) {
    struct merge_args *parametros = (struct merge_args *)argumentos;
    int indice_thread = parametros->idxThread;
    int total_threads_atuais = parametros->nThreads;
    FILE *arquivo_entrada_1 = parametros->fp1;
    FILE *arquivo_entrada_2 = parametros->fp2;

    printf("Thread %d iniciando merge.\n", indice_thread);

    char nome_arquivo_saida[16];
    sprintf(nome_arquivo_saida, "%d%d.txt", total_threads_atuais, indice_thread);

    FILE *arquivo_saida = fopen(nome_arquivo_saida, "w");
    if (arquivo_saida == NULL) {
        perror("Erro ao criar arquivo de saída");
        pthread_exit(NULL);
    }

    char linha1[TAM_MAX_ITEM];
    char linha2[TAM_MAX_ITEM];

    int leu1 = fgets(linha1, TAM_MAX_ITEM, arquivo_entrada_1) != NULL;
    int leu2 = fgets(linha2, TAM_MAX_ITEM, arquivo_entrada_2) != NULL;

    if (leu1) {
        linha1[strcspn(linha1, "\r\n")] = '\0';
    }

    if (leu2) {
        linha2[strcspn(linha2, "\r\n")] = '\0';
    }

    while (leu1 && leu2) {
        if (strcmp(linha1, linha2) <= 0) {
            fprintf(arquivo_saida, "%s\n", linha1);
            leu1 = fgets(linha1, TAM_MAX_ITEM, arquivo_entrada_1) != NULL;
            if (leu1) {
                linha1[strcspn(linha1, "\r\n")] = '\0';
            }
        } else {
            fprintf(arquivo_saida, "%s\n", linha2);
            leu2 = fgets(linha2, TAM_MAX_ITEM, arquivo_entrada_2) != NULL;
            if (leu2) {
                linha2[strcspn(linha2, "\r\n")] = '\0';
            }
        }
    }

    while (leu1) {
        fprintf(arquivo_saida, "%s\n", linha1);
        leu1 = fgets(linha1, TAM_MAX_ITEM, arquivo_entrada_1) != NULL;
        if (leu1) {
            linha1[strcspn(linha1, "\r\n")] = '\0';
        }
    }

    while (leu2) {
        fprintf(arquivo_saida, "%s\n", linha2);
        leu2 = fgets(linha2, TAM_MAX_ITEM, arquivo_entrada_2) != NULL;
        if (leu2) {
            linha2[strcspn(linha2, "\r\n")] = '\0';
        }
    }

    fclose(arquivo_entrada_1);
    fclose(arquivo_entrada_2);
    fclose(arquivo_saida);

    FILE *arquivo_retorno = fopen(nome_arquivo_saida, "r");
    if (arquivo_retorno == NULL) {
        perror("Erro ao reabrir arquivo de saída para leitura");
        pthread_exit(NULL);
    }

    pthread_exit(arquivo_retorno);
}
