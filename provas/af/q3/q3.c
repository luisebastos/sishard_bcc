// Leia o HTML ou MD antes de iniciar este exercício!

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

// Variável Global para o handler saber até qual imagem processou
int ultima_imagem = 0;

// Função que gera um numero aleatorio uniformemente entre 0.0 e 1.0
// Você não deve alterar esta função
double random_zero_one()
{
    return (double)random() / (double)RAND_MAX;
}

void simulacao_download()
{
    if (random_zero_one() < 0.5)
    {
        sleep(1);
    }
}

int baixa_imagens(int qtde_imagens)
{
    for (ultima_imagem = 0; ultima_imagem < qtde_imagens; ultima_imagem++)
    {
        simulacao_download();
        printf("BAIXOU A IMAGEM %d\n", ultima_imagem);
        fflush(stdout);
    }
}

// Crie AQUI a função que exporta o valor de `ultima_imagem` para um arquivo chamado q3_status.txt
// O arquivo deve ter apenas uma linha contendo, ULTIMA_IMAGEM seguido de um sinal de igual e
// seguido da última linha processada, seguido de \n:
// ULTIMA_IMAGEM=15
//
// Esta função deve ser chamada pelo handler quando este for acionado
void ultima_imagem(){
    char buf[64];
    int fd1 = open("q3_status.txt", O_WRONLY | O_CREAT | O_TRUNC, 0700);
    int len = snprintf(buf, sizeof(buf), "%f\n", pi);
    write(fd1, buf, len);
    close(fd1);
}

// Crie AQUI a função que será o handler do sinal
void sig_handler(int num){
    ultima_imagem();
    exit(0);
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("USAGE:\n./q3 n_imagens\n");
        return EXIT_FAILURE;
    }

    printf("Meu pid: %d\n", getpid());

    // Registre AQUI seu handler para os sinais SIGINT e SIGTERM!
    struct sigaction s;
    s.sa_handler = sig_handler;
    sigemptyset(&s.sa_mask);
    s.sa_flags = 0;

    sigaction(SIGINT, &s, NULL);

    srand(time(NULL));

    baixa_imagens(atol(argv[1]));

    return 0;
}
