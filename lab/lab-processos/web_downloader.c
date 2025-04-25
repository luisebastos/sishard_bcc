/*
Compile:
   gcc web_downloader.c -o web_downloader -Wall -Wno-unused-result -Og -g -lcurl
Teste:
   ./web_downloader http://exemplo.com/pagina.html
   ./web_downloader -f lista_download.txt
Referência:
   https://curl.se/libcurl/c/simple.html
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

// auxilio do ChatGPT para refatorar essa função 
void gerar_nome(const char *host, const char *path, char *dest, size_t max_len) {
    snprintf(dest, max_len, "%s%s", host, path);
    char *ultimo_ponto = strrchr(dest, '.');
    char *barra_depois = strrchr(dest, '/');
    if (barra_depois && barra_depois > ultimo_ponto)
        ultimo_ponto = NULL;

    for (size_t i = 0; dest[i] != '\0'; i++) {
        if (&dest[i] == ultimo_ponto) continue;
        if (dest[i] == '/' || dest[i] == '.') dest[i] = '_';
    }
}


size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    int fd = *(int *)userdata;
    return write(fd, ptr, size * nmemb);
}

// auxilio do ChatGPT para montar a lógica do parsing de URL com `curl_url_*`
void baixar_url(const char *url) {
    CURLU *urlp = curl_url();
    char *host = NULL, *path = NULL;

    if (curl_url_set(urlp, CURLUPART_URL, url, 0) != CURLUE_OK) {
        dprintf(STDERR_FILENO, "Erro ao fazer parsing da URL: %s\n", url);
        curl_url_cleanup(urlp);
        _exit(1);
    }

    curl_url_get(urlp, CURLUPART_HOST, &host, 0);
    curl_url_get(urlp, CURLUPART_PATH, &path, 0);
    if (!host || !path) {
        dprintf(STDERR_FILENO, "Erro: URL inválida: %s\n", url);
        curl_free(host);
        curl_free(path);
        curl_url_cleanup(urlp);
        _exit(1);
    }

    char nome_arquivo[1024];
    gerar_nome(host, path, nome_arquivo, sizeof(nome_arquivo));

    int fd = open(nome_arquivo, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Erro ao abrir arquivo");
        curl_free(host);
        curl_free(path);
        curl_url_cleanup(urlp);
        _exit(1);
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        write(STDERR_FILENO, "Erro ao inicializar curl\n", 26);
        close(fd);
        curl_free(host);
        curl_free(path);
        curl_url_cleanup(urlp);
        _exit(1);
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &fd);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    CURLcode resultado = curl_easy_perform(curl);

    if (resultado == CURLE_OK) {
        dprintf(STDOUT_FILENO, "%s baixada com sucesso!\n", url);
    } else {
        dprintf(STDERR_FILENO, "Erro no download [%s]: %s\n", url, curl_easy_strerror(resultado));
    }

    close(fd);
    curl_easy_cleanup(curl);
    curl_free(host);
    curl_free(path);
    curl_url_cleanup(urlp);
    _exit(0);
}

// auxilio do ChatGPT para implementar a leitura de um arquivo com `read()` em vez de `fgets()'
void ler_urls_posix(const char *nome_arquivo) {
    int fd = open(nome_arquivo, O_RDONLY);
    if (fd < 0) {
        perror("Erro ao abrir o arquivo da lista");
        exit(1);
    }

    char buffer[4096];
    char linha[2048];
    ssize_t bytes_lidos;
    size_t start = 0, total = 0;

    while ((bytes_lidos = read(fd, buffer + total, sizeof(buffer) - total)) > 0) {
        total += bytes_lidos;

        for (size_t i = start; i < total; i++) {
            if (buffer[i] == '\n') {
                size_t tam = i - start;
                if (tam >= sizeof(linha)) tam = sizeof(linha) - 1;
                memcpy(linha, buffer + start, tam);
                linha[tam] = '\0';

                pid_t pid = fork();
                if (pid == 0) {
                    baixar_url(linha);
                }

                start = i + 1;
            }
        }

        if (start < total) {
            memmove(buffer, buffer + start, total - start);
            total = total - start;
        } else {
            total = 0;
        }
        start = 0;
    }

    close(fd);

    while (wait(NULL) > 0);
}

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "-f") != 0) {
        baixar_url(argv[1]);
    } else if (argc == 3 && strcmp(argv[1], "-f") == 0) {
        ler_urls_posix(argv[2]);
    } else {
        const char *msg_erro = "Uso:\n  ./web_downloader URL\n  ./web_downloader -f lista.txt\n";
        write(STDERR_FILENO, msg_erro, strlen(msg_erro));
        return 1;
    }

    return 0;
}
