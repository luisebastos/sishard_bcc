// Leia o HTML ou MD antes de iniciar este exercício!

// inclua libs aqui!
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

char *read_word(int fd);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct
{
    int id_thread;   // identificador da thread
    int *count_cpfs; // para as threads saberem onde guardar o resultado!
    int file_desc;   // descritor do arquivo
} t_arg;

#define MAX_LINE_LENGTH 256

/*
 * Função que verifica se um char é numérico
 */
int isnum(char c)
{
    return c >= '0' && c <= '9';
}

/*
 * Função que verifica se uma palavra é um cpf válido
 * do ponto de vista do formado DDD.DDD.DDD-DD
 * onde D é um dígito
 *
 * ENTRADA:
 * char *txt: cpf a ser validado
 *
 * SAIDA:
 *   (int) 0: o CPF é válido
 *   (int) 1: Formato inválido!
 *   (int) 2: Formato ok mas regra inválida!
 */
int validade_cpf(char *txt)
{
    int qtde = 0;
    for (qtde = 0; txt[qtde] != '\0'; qtde++)
        ;
    if (qtde != 14)
    {
        return 1;
    }
    if (!(isnum(txt[0]) && isnum(txt[1]) && isnum(txt[2]) && txt[3] == '.' &&
        isnum(txt[4]) && isnum(txt[5]) && isnum(txt[6]) && txt[7] == '.' &&
        isnum(txt[8]) && isnum(txt[9]) && isnum(txt[10]) && txt[11] == '-' &&
        isnum(txt[12]) && isnum(txt[13])))
        return 1;
    
    if (txt[0] == txt[1] && txt[0] == txt[2] && txt[0] == txt[4] && txt[0] == txt[5] &&
        txt[0] == txt[6] && txt[0] == txt[8] && txt[0] == txt[9] && txt[0] == txt[10] &&
        txt[0] == txt[12] && txt[0] == txt[13])
        return 2; //todos dígitos iguais
    
    int dig1 = ((((txt[0]-'0')*10 + (txt[1]-'0')*9 + (txt[2]-'0')*8 + (txt[4]-'0')*7 +
                  (txt[5]-'0')*6 + (txt[6]-'0')*5 + (txt[8]-'0')*4 + (txt[9]-'0')*3 +
                  (txt[10]-'0')*2)*10)%11)%10;

    int dig2 = ((((txt[0]-'0')*11 + (txt[1]-'0')*10 + (txt[2]-'0')*9 + (txt[4]-'0')*8 +
                  (txt[5]-'0')*7 + (txt[6]-'0')*6 + (txt[8]-'0')*5 + (txt[9]-'0')*4 +
                  (txt[10]-'0')*3 + (txt[12]-'0')*2)*10)%11)%10;
    
    if(dig1 == (txt[12]-'0') && dig2 == (txt[13]-'0')) return 0; //regra ok
    else return 2; //cpf inválido
}

/*
 * Faz validação de cpf em Thread.
 *
 * A thread recebe um descritor de arquivo e deve ler
 * palavras dele enquanto o arquivo não acabar!
 *
 */
void *cpf_validation_thread(void *_arg)
{
    t_arg *arg = _arg;

    while (1)
    {
        // Tenta ler uma palavra do arquivo
        char *palavra = read_word(arg->file_desc);; 
        pthread_mutex_lock(&mutex); 
        palavra = read_word(arg->file_desc);
        pthread_mutex_unlock(&mutex);

        if (palavra == NULL)
        {
            // FUNCAO read_word NAO IMPLEMENTADA OU FINAL DO ARQUIVO!
            break;
        }

        printf("THREAD %02d VAI PROCESSAR PALAVRA [%s]\n", arg->id_thread, palavra);
        fflush(stdout);

        int ret = validade_cpf(palavra);
        if (ret == 0)
        {
            pthread_mutex_lock(&mutex); 
            *arg->count_cpfs = *arg->count_cpfs + 1;
            pthread_mutex_unlock(&mutex);
        }
        free(palavra);
    }
    return NULL; 
}

/* Função que lê uma palavra de um arquivo.
 * Considere que o arquivo contém apenas uma linha, sem \n no final.
 * A linha possui diversas palavras, separadas por um único espaço
 *
 * Entradas:
 *   int fd: descritor do arquivo
 *
 * Saída:
 *   Ponteiro (char *) para uma string com a palavra lida. Caso o arquivo termine, retorne NULL.
 */
char *read_word(int fd)
{
    char buffer[1];
    char *palavra = malloc(MAX_LINE_LENGTH * sizeof(char));
    int pos = 0;
    int length = 0;

    while (1)
    {
        length = read(fd, buffer, 1);
        // printf("length: %d\n %c", length, buffer[0]);
        if (length == 0)
        {
            if (pos == 0)
            {
                free(palavra);
                return NULL; // End of file reached
            }
            else
            {
                palavra[pos] = '\0';
                return palavra;
            }
        }
        else
        {
            if (buffer[0] == ' ')
            {
                palavra[pos] = '\0';
                return palavra;
            }
            else
            {
                palavra[pos] = buffer[0];
                pos++;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("USAGE:\n./q4 <n_threads> <file_name>\n");
        return EXIT_FAILURE;
    }

    int n_threads = atoi(argv[1]); // Info da linha de comando
    char *file_name = argv[2];     // Info da linha de comando

    int fd1 = open(file_name, O_RDONLY); // Abre o arquivo

    if (fd1 == -1)
    {
        fprintf(stderr, "Falha ao abrir o arquivo!\n");
        exit(EXIT_FAILURE);
    }

    // Inicializa contador de CPFs encontrados
    int count_cpfs = 0;

    // Aloca espaço para tids e args das threads
    pthread_t *tids = malloc(sizeof(pthread_t) * n_threads);
    t_arg **args = malloc(sizeof(t_arg *) * n_threads);

    for (int i = 0; i < n_threads; i++)
    {

        t_arg *arg = malloc(sizeof(t_arg));
        arg->file_desc = fd1; // Thread recebe o file descriptor
        arg->count_cpfs = &count_cpfs;
        arg->id_thread = i;
        args[i] = arg;

        pthread_create(&tids[i], NULL, cpf_validation_thread, arg);
        printf("CRIOU THREAD %02d\n", i);
    }

    for (int i = 0; i < n_threads; i++)
    {
        pthread_join(tids[i], NULL);
        printf("THREAD %02d TERMINOU\n", i);
    }

    printf("RESULTADO:\nFORAM ENCONTRADOS %02d CPFS VALIDOS\n", count_cpfs);

    for (int i = 0; i < n_threads; i++)
    {
        free(args[i]);
    }

    close(fd1);
    free(tids);
    free(args);

    pthread_mutex_destroy(&mutex); 

    return 0;
}
