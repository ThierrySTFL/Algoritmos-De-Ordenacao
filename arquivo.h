#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stddef.h>
#include "dados.h"

int ler_numeros(const char *caminho, int **dados, size_t *quantidade);
int escrever_saida(const char *caminho, const int *dados, size_t quantidade, Estatisticas estatisticas);
int carregar_entrada_padrao(size_t n, int **dados, size_t *lidos, TipoEntrada *tipo);

#endif