#ifndef DADOS_H
#define DADOS_H

#include <stddef.h>

typedef enum {
    ENTRADA_ALEATORIA,
    ENTRADA_CRESCENTE,
    ENTRADA_DECRESCENTE
} TipoEntrada;

typedef struct {
    long long comparacoes;
    long long movimentacoes;
    double tempo_ms;
} Estatisticas;

// Contadores globais usados pelas funções de ordenação
extern Estatisticas g_estatisticas;

static inline void zerar_estatisticas(void) {
    g_estatisticas.comparacoes = 0;
    g_estatisticas.movimentacoes = 0;
}

static inline void contar_comparacao(void) {
    g_estatisticas.comparacoes++;
}

static inline void contar_movimentacao(void) {
    g_estatisticas.movimentacoes++;
}

void gerar_entrada(int *destino, int n, TipoEntrada tipo);
const char *entrada_para_string(TipoEntrada tipo);
Estatisticas medir_algoritmo(void (*sort_fn)(int *, int), const int *entrada, int n);
void medir_todos_algoritmos(const int *entrada, int n, TipoEntrada tipo);

#endif
