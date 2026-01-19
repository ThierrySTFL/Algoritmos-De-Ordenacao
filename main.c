// Programa principal: executa cada algoritmo de ordenação para entradas de
// tamanhos 10.000, 100.000 e 500.000 (geradas pelo gera.c) e grava os vetores
// ordenados + métricas em arquivos saida_<funcao>_<tamanho>.txt.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "arquivo.h"
#include "dados.h"
#include "sort.h"

typedef void (*sort_fn)(int *, int);

typedef struct {
    const char *nome;
    sort_fn fn;
} Algoritmo;

static double diff_ms(clock_t inicio, clock_t fim) {
    return ((double)(fim - inicio) * 1000.0) / (double)CLOCKS_PER_SEC;
}

// Carrega o arquivo de entrada correspondente ao tamanho desejado.
static int carregar_entrada(size_t tamanho, char *caminho_usado, size_t len, int **dados, size_t *lidos) {
    if (!caminho_usado || !dados || !lidos) {
        return -1;
    }

    char candidatos[4][64];
    snprintf(candidatos[0], sizeof(candidatos[0]), "entrada_%zu.txt", tamanho);
    snprintf(candidatos[1], sizeof(candidatos[1]), "saida_%zu.txt", tamanho);
    snprintf(candidatos[2], sizeof(candidatos[2]), "entrada.txt");
    snprintf(candidatos[3], sizeof(candidatos[3]), "saida.txt");

    for (size_t i = 0; i < 4; i++) {
        FILE *teste = fopen(candidatos[i], "r");
        if (!teste) {
            continue;
        }
        fclose(teste);

        if (ler_numeros(candidatos[i], dados, lidos) == 0) {
            if (*lidos < tamanho) {
                free(*dados);
                *dados = NULL;
                *lidos = 0;
                continue;
            }
            strncpy(caminho_usado, candidatos[i], len);
            caminho_usado[len - 1] = '\0';
            return 0;
        }
    }

    return -1;
}

static Estatisticas rodar_algoritmo(const Algoritmo *alg, const int *entrada, size_t tamanho) {
    Estatisticas stats = {0, 0, 0.0};
    if (!alg || !entrada || tamanho == 0) {
        return stats;
    }

    int *copia = malloc(tamanho * sizeof(int));
    if (!copia) {
        perror("falha ao alocar vetor para ordenacao");
        return stats;
    }
    memcpy(copia, entrada, tamanho * sizeof(int));

    zerar_estatisticas();
    clock_t inicio = clock();
    alg->fn(copia, (int)tamanho);
    clock_t fim = clock();

    stats.comparacoes = g_estatisticas.comparacoes;
    stats.movimentacoes = g_estatisticas.movimentacoes;
    stats.tempo_ms = diff_ms(inicio, fim);

    char nome_saida[128];
    snprintf(nome_saida, sizeof(nome_saida), "saida_%s_%zu.txt", alg->nome, tamanho);
    if (escrever_saida(nome_saida, copia, tamanho, stats) != 0) {
        fprintf(stderr, "nao foi possivel gravar %s\n", nome_saida);
    }

    free(copia);
    return stats;
}

int main(void) {
    const size_t tamanhos[] = {10000, 100000, 500000};
    const size_t total_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);

    const Algoritmo algoritmos[] = {
        {"Bolha", Bolha},
        {"BolhaComCriterioDeParada", BolhaComCriterioDeParada},
        {"InsercaoDireta", InsercaoDireta},
        {"InsercaoBinaria", InsercaoBinaria},
        {"InsercaoTernaria", InsercaoTernaria},
        {"ShellSort", ShellSort},
        {"SelecaoDireta", SelecaoDireta},
        {"HeapSort", HeapSort},
        {"QuickSort", QuickSort},
        {"QuickSortCentro", QuickSortCentro},
        {"QuickSortFim", QuickSortFim},
        {"QuickSortMediana", QuickSortMediana},
        {"MergeSort", MergeSort},
        {"RadixSort", RadixSort},
        {"BucketSort", BucketSort},
    };
    const size_t total_algoritmos = sizeof(algoritmos) / sizeof(algoritmos[0]);

    for (size_t t = 0; t < total_tamanhos; t++) {
        size_t tamanho = tamanhos[t];
        char arquivo_entrada[64];
        int *dados = NULL;
        size_t lidos = 0;

        if (carregar_entrada(tamanho, arquivo_entrada, sizeof(arquivo_entrada), &dados, &lidos) != 0) {
            fprintf(stderr, "Arquivo de entrada para %zu itens nao encontrado (esperado algo como entrada_%zu.txt ou saida_%zu.txt).\n",
                    tamanho, tamanho, tamanho);
            continue;
        }

        printf("Processando %zu numeros a partir de \"%s\" (%zu lidos)...\n", tamanho, arquivo_entrada, lidos);

        for (size_t a = 0; a < total_algoritmos; a++) {
            Estatisticas e = rodar_algoritmo(&algoritmos[a], dados, tamanho);
            printf("  %-24s comparacoes:%10lld movimentacoes:%10lld tempo:%10.3f ms\n",
                   algoritmos[a].nome, e.comparacoes, e.movimentacoes, e.tempo_ms);
        }

        free(dados);
    }

    return 0;
}
