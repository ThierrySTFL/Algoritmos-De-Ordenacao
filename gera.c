#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void mostrar_uso(const char *prog) { // mostra como usar o programa
    fprintf(stderr,
            "Uso: %s [-a|-c|-d] n\n"
            "  -a, --aleatorio   gera n numeros aleatorios\n"
            "  -c, --crescente   gera numeros em ordem crescente\n"
            "  -d, --decrescente gera numeros em ordem decrescente\n",
            prog);
    exit(EXIT_FAILURE);
}

long ler_n_positivo(const char *prog, const char *texto) {
    char *fim = NULL;
    long n = strtol(texto, &fim, 10);

    // Vai ser invalido se não converteu nada ou sobrou lixo ou n <= 0 
    if (fim == texto || *fim != '\0' || n <= 0) {
        fprintf(stderr, "Valor de n invalido: %s\n", texto);
        mostrar_uso(prog);
    }
    return n;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        mostrar_uso(argv[0]);
    }

    const char *opcao = argv[1];
    long n = ler_n_positivo(argv[0], argv[2]);

    const char *nome_arquivo = "saida.txt";
    FILE *out = fopen(nome_arquivo, "w");
    if (out == NULL) {
        perror("Erro ao criar arquivo de saida");
        return EXIT_FAILURE;
    }

    // crescente
    if (strcmp(opcao, "-c") == 0 || strcmp(opcao, "--crescente") == 0) { 
        for (long i = 1; i <= n; i++) {
            fprintf(out, "%ld\n", i);
        }
        fclose(out);
        return EXIT_SUCCESS;
    }

    // Decrescente
    if (strcmp(opcao, "-d") == 0 || strcmp(opcao, "--decrescente") == 0) {
        for (long i = n; i >= 1; i--) {
            fprintf(out, "%ld\n", i);
        }
        fclose(out);
        return EXIT_SUCCESS;
    }

    // Aleatório
    if (strcmp(opcao, "-a") == 0 || strcmp(opcao, "--aleatorio") == 0) {
        srand((unsigned)time(NULL));
        for (long i = 0; i < n; i++) {
            long valor = (long)(rand() % n) + 1;
            fprintf(out, "%ld\n", valor);
        }
        fclose(out);
        return EXIT_SUCCESS;
    }

    fclose(out);
    mostrar_uso(argv[0]);
    return EXIT_FAILURE;
}