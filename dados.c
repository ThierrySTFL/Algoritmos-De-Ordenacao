#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dados.h"
#include "sort.h"

Estatisticas g_estatisticas = {0, 0, 0.0};

static double diff_ms(clock_t inicio, clock_t fim) {
    return ((double)(fim - inicio) * 1000.0) / (double)CLOCKS_PER_SEC;
}

void gerar_entrada(int *destino, int n, TipoEntrada tipo) {
    static int ja_inicializou_seed = 0;
    if (!destino || n <= 0) {
        return;
    }

    if (!ja_inicializou_seed) {
        srand((unsigned)time(NULL));
        ja_inicializou_seed = 1;
    }

    switch (tipo) {
    case ENTRADA_CRESCENTE:
        for (int i = 0; i < n; i++) {
            destino[i] = i + 1;
        }
        break;
    case ENTRADA_DECRESCENTE:
        for (int i = 0; i < n; i++) {
            destino[i] = n - i;
        }
        break;
    case ENTRADA_ALEATORIA:
    default:
        for (int i = 0; i < n; i++) {
            destino[i] = (rand() % n) + 1;
        }
        break;
    }
}

const char *entrada_para_string(TipoEntrada tipo) {
    switch (tipo) {
    case ENTRADA_ALEATORIA:
        return "aleatoria";
    case ENTRADA_CRESCENTE:
        return "crescente";
    case ENTRADA_DECRESCENTE:
        return "decrescente";
    default:
        return "desconhecida";
    }
}

Estatisticas medir_algoritmo(void (*sort_fn)(int *, int), const int *entrada, int n) {
    Estatisticas resultado = {0, 0, 0.0};
    if (!sort_fn || !entrada || n <= 0) {
        return resultado;
    }

    int *copia = malloc((size_t)n * sizeof(int));
    if (!copia) {
        perror("falha ao alocar vetor de teste");
        return resultado;
    }
    memcpy(copia, entrada, (size_t)n * sizeof(int));

    zerar_estatisticas();

    clock_t inicio = clock();
    sort_fn(copia, n);
    clock_t fim = clock();

    resultado.comparacoes = g_estatisticas.comparacoes;
    resultado.movimentacoes = g_estatisticas.movimentacoes;
    resultado.tempo_ms = diff_ms(inicio, fim);

    free(copia);
    return resultado;
}

static void imprimir_resultado(const char *nome, TipoEntrada tipo, Estatisticas e) {
    printf("%-24s %-11s comparacoes:%10lld movimentacoes:%10lld tempo:%10.3f ms\n",
           nome, entrada_para_string(tipo), e.comparacoes, e.movimentacoes, e.tempo_ms);
}

void medir_todos_algoritmos(const int *entrada, int n, TipoEntrada tipo) {
    if (!entrada || n <= 0) {
        return;
    }

    struct {
        const char *nome;
        void (*fn)(int *, int);
    } algoritmos[] = {
        {"Bolha", Bolha},
        {"Bolha (parada)", BolhaComCriterioDeParada},
        {"Insercao direta", InsercaoDireta},
        {"Insercao binaria", InsercaoBinaria},
        {"Insercao ternaria", InsercaoTernaria},
        {"ShellSort", ShellSort},
        {"Selecao direta", SelecaoDireta},
        {"HeapSort", HeapSort},
        {"QuickSort", QuickSort},
        {"QuickSort (centro)", QuickSortCentro},
        {"QuickSort (fim)", QuickSortFim},
        {"QuickSort (mediana)", QuickSortMediana},
        {"MergeSort", MergeSort},
        {"RadixSort", RadixSort},
        {"BucketSort", BucketSort},
    };

    size_t total = sizeof(algoritmos) / sizeof(algoritmos[0]);
    for (size_t i = 0; i < total; i++) {
        Estatisticas e = medir_algoritmo(algoritmos[i].fn, entrada, n);
        imprimir_resultado(algoritmos[i].nome, tipo, e);
    }
}
