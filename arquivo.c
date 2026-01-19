// Rotinas para carregar arquivos de entrada e salvar resultados.
#include <stdio.h>
#include <stdlib.h>
#include "arquivo.h"

int ler_numeros(const char *caminho, int **dados, size_t *quantidade) {
    if (!caminho || !dados || !quantidade) {
        return -1;
    }

    FILE *in = fopen(caminho, "r");
    if (!in) {
        perror("falha ao abrir arquivo de entrada");
        return -1;
    }

    size_t total = 0;
    int valor = 0;
    while (fscanf(in, "%d", &valor) == 1) {
        total++;
    }

    if (ferror(in) || total == 0) {
        fclose(in);
        return -1;
    }

    if (fseek(in, 0L, SEEK_SET) != 0) {
        fclose(in);
        return -1;
    }

    int *vetor = (int *)malloc(total * sizeof(int));
    if (!vetor) {
        fclose(in);
        perror("falha ao alocar memoria para leitura");
        return -1;
    }

    size_t idx = 0;
    while (idx < total && fscanf(in, "%d", &valor) == 1) {
        vetor[idx++] = valor;
    }

    fclose(in);

    if (idx != total) {
        free(vetor);
        return -1;
    }

    *dados = vetor;
    *quantidade = total;
    return 0;
}

int escrever_saida(const char *caminho, const int *dados, size_t quantidade, Estatisticas estatisticas) {
    if (!caminho || !dados || quantidade == 0) {
        return -1;
    }

    FILE *out = fopen(caminho, "w");
    if (!out) {
        perror("falha ao criar arquivo de saida");
        return -1;
    }

    for (size_t i = 0; i < quantidade; i++) {
        fprintf(out, "%d\n", dados[i]);
    }

    fprintf(out, "comparacoes:%lld\n", estatisticas.comparacoes);
    fprintf(out, "movimentacoes:%lld\n", estatisticas.movimentacoes);
    fprintf(out, "tempo_ms:%.3f\n", estatisticas.tempo_ms);

    fclose(out);
    return 0;
}
