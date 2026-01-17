#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void uso(const char *prog) {
    fprintf(stderr,
            "Uso: %s [-a|-c|-d] n\n"
            "  -a, --aleatorio   gera n numeros aleatorios\n"
            "  -c, --crescente   gera numeros em ordem crescente\n"
            "  -d, --decrescente gera numeros em ordem decrescente\n",
            prog);
    exit(EXIT_FAILURE);
}

static long ler_n(const char *prog, const char *arg) {
    char *endptr = NULL;
    long n = strtol(arg, &endptr, 10);
    if (endptr == arg || *endptr != '\0' || n <= 0) {
        fprintf(stderr, "Valor de n invalido: %s\n", arg);
        uso(prog);
    }
    return n;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        uso(argv[0]);
    }

    const char *opcao = argv[1];
    long n = ler_n(argv[0], argv[2]);

    const char *arquivo_saida = "saida.txt";
    FILE *out = fopen(arquivo_saida, "w");
    if (!out) {
        perror("Erro ao criar arquivo de saida");
        return EXIT_FAILURE;
    }

    if (strcmp(opcao, "-c") == 0 || strcmp(opcao, "--crescente") == 0) {
        for (long i = 1; i <= n; i++) {
            fprintf(out, "%ld\n", i);
        }
        fclose(out);
        return 0;
    }

    if (strcmp(opcao, "-d") == 0 || strcmp(opcao, "--decrescente") == 0) {
        for (long i = n; i >= 1; i--) {
            fprintf(out, "%ld\n", i);
        }
        fclose(out);
        return 0;
    }

    if (strcmp(opcao, "-a") == 0 || strcmp(opcao, "--aleatorio") == 0) {
        srand((unsigned)time(NULL));
        for (long i = 0; i < n; i++) {
            fprintf(out, "%ld\n", ((long)(rand() % n)) + 1);
        }
        fclose(out);
        return 0;
    }

    uso(argv[0]);
    return 0;
}
