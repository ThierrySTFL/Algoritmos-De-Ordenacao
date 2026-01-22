#ifndef DADOS_H
#define DADOS_H

#include <stddef.h>

// entradas que serao geradas
typedef enum {
    ENTRADA_ALEATORIA,
    ENTRADA_CRESCENTE,
    ENTRADA_DECRESCENTE
} TipoEntrada;

// dados coletados serao esses
typedef struct {
    long long comparacoes;
    long long movimentacoes;
    double tempo_ms;
} Estatisticas;


// variavel global pra contar operações.
extern Estatisticas g_estatisticas;

static inline void zerar_estatisticas(void) {
    g_estatisticas.comparacoes = 0;
    g_estatisticas.movimentacoes = 0;
}

/* Funções simples para incrementar os contadores */
static inline void contar_comparacao(void) {
    g_estatisticas.comparacoes++;
}

static inline void contar_movimentacao(void) {
    g_estatisticas.movimentacoes++;
}

// Funciona como se fosse uma API pra medição

void gerar_entrada(int *destino, int n, TipoEntrada tipo); // gera uma entrada no vetor destino
const char *entrada_para_string(TipoEntrada tipo); // converte tipo de entrada para string
Estatisticas medir_algoritmo(void (*sort_fn)(int *, int), const int *entrada, int n); // mede um algoritmo de ordenação
void medir_todos_algoritmos(const int *entrada, int n, TipoEntrada tipo); // mede todos os algoritmos de ordenação

#endif