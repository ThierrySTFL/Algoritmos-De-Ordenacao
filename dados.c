#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dados.h"
#include "arquivo.h"
#include "sort.h"

Estatisticas g_estatisticas = {0, 0, 0.0}; // inicializa variavel global

static double ms_entre(clock_t inicio, clock_t fim) { // calcula ms entre dois clocks, nao achei jeito melhor
    return ((double)(fim - inicio) * 1000.0) / (double)CLOCKS_PER_SEC;
}

const char *entrada_para_string(TipoEntrada tipo) { // converte tipo de entrada para string
    switch (tipo) {
        case ENTRADA_ALEATORIA:   return "aleatoria";
        case ENTRADA_CRESCENTE:   return "crescente";
        case ENTRADA_DECRESCENTE: return "decrescente";
        default:                  return "desconhecida";
    }
}

// mede um algoritmo de ordenação
Estatisticas medir_algoritmo(void (*sort_fn)(int *, int),
                            const int *entrada,
                            int n) {
    Estatisticas resultado = {0, 0, 0.0};

    if (sort_fn == NULL || entrada == NULL || n <= 0) {
        return resultado;
    }

    int *copia = (int *)malloc((size_t)n * sizeof(int));
    if (copia == NULL) {
        return resultado;
    }

    memcpy(copia, entrada, (size_t)n * sizeof(int));

    // reseta contadores (todos golbais)
    g_estatisticas.comparacoes = 0;
    g_estatisticas.movimentacoes = 0;
    g_estatisticas.tempo_ms = 0.0;

    clock_t inicio = clock();
    sort_fn(copia, n);
    clock_t fim = clock();

    resultado.comparacoes = g_estatisticas.comparacoes;
    resultado.movimentacoes = g_estatisticas.movimentacoes;
    resultado.tempo_ms = ms_entre(inicio, fim);

    free(copia);
    return resultado;
}

// imprime o resultado de um algoritmo
static void imprimir_resultado(const char *nome, TipoEntrada tipo, Estatisticas e) {
    printf("%-24s %-11s %15lld %15lld %12.3f\n",
           nome,
           entrada_para_string(tipo),
           e.comparacoes,
           e.movimentacoes,
           e.tempo_ms);
}

// mede todos os algoritmos de ordenação
void medir_todos_algoritmos(const int *entrada, int n, TipoEntrada tipo) {
    printf("%-24s %-11s %15s %15s %12s\n",
       "Algoritmo", "Entrada", "Comparacoes", "Movimentacoes", "Tempo(ms)");
    printf("%-24s %-11s %15s %15s %12s\n",
       "--------",  "-------", "-----------", "-------------", "--------");
    if (entrada == NULL || n <= 0) return;

    typedef struct {
        const char *nome; // para imprimir
        const char *tag;  // para nome do arquivo (sem espaços)
        void (*fn)(int *, int);
    } Algoritmo;

    Algoritmo algoritmos[] = {
        {"Bolha",               "Bolha",               Bolha},
        {"Bolha (parada)",      "Bolha_parada",        BolhaComCriterioDeParada},
        {"Insercao direta",     "Insercao_direta",     InsercaoDireta},
        {"Insercao binaria",    "Insercao_binaria",    InsercaoBinaria},
        {"Insercao ternaria",   "Insercao_ternaria",   InsercaoTernaria},
        {"ShellSort",           "ShellSort",           ShellSort},
        {"Selecao direta",      "Selecao_direta",      SelecaoDireta},
        {"HeapSort",            "HeapSort",            HeapSort},
        {"QuickSort",           "QuickSort",           QuickSort},
        {"QuickSort (centro)",  "QuickSort_centro",    QuickSortCentro},
        {"QuickSort (fim)",     "QuickSort_fim",       QuickSortFim},
        {"QuickSort (mediana)", "QuickSort_mediana",   QuickSortMediana},
        {"MergeSort",           "MergeSort",           MergeSort},
        {"RadixSort",           "RadixSort",           RadixSort},
        {"BucketSort",          "BucketSort",          BucketSort},
    };

    int total = sizeof(algoritmos) / sizeof(algoritmos[0]); // número de algoritmos

    for (int i = 0; i < total; i++) { // para cada algoritmo
        int *copia = (int *)malloc((size_t)n * sizeof(int));
        if (copia == NULL) {
            fprintf(stderr, "Falha ao alocar copia para %s\n", algoritmos[i].nome);
            continue;
        }
        memcpy(copia, entrada, (size_t)n * sizeof(int));

        // reseta contadores globais
        g_estatisticas.comparacoes = 0;
        g_estatisticas.movimentacoes = 0;
        g_estatisticas.tempo_ms = 0.0;

        clock_t inicio = clock();
        algoritmos[i].fn(copia, n);
        clock_t fim = clock();

        Estatisticas e;
        e.comparacoes = g_estatisticas.comparacoes;
        e.movimentacoes = g_estatisticas.movimentacoes;
        e.tempo_ms = ms_entre(inicio, fim);

        // grava arquivo de saída
        char nome_saida[128];
        snprintf(nome_saida, sizeof(nome_saida), "saida_%s_%d.txt", algoritmos[i].tag, n); // nome formatado
        if (escrever_saida(nome_saida, copia, (size_t)n, e) != 0) { // valida se gravou
            fprintf(stderr, "Nao foi possivel gravar %s\n", nome_saida); 
        }

        // imprime no console
        imprimir_resultado(algoritmos[i].nome, tipo, e);

        free(copia);
    }
}