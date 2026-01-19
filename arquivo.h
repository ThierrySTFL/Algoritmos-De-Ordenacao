// Utilidades para ler entradas e gravar os resultados dos algoritmos.
#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stddef.h>
#include "dados.h"

// Lê todos os inteiros de um arquivo de texto (um por linha).
// Aloca o vetor de saída e devolve em *dados e *quantidade.
// Retorna 0 em sucesso, -1 em erro.
int ler_numeros(const char *caminho, int **dados, size_t *quantidade);

// Grava os números ordenados e, ao final, as métricas da execução.
// Retorna 0 em sucesso, -1 em erro.
int escrever_saida(const char *caminho, const int *dados, size_t quantidade, Estatisticas estatisticas);

#endif
