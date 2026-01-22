#include <stdio.h>
#include <stdlib.h>

#include "arquivo.h"
#include "dados.h"

int main(void) {
    const size_t tamanhos[] = { 10000, 100000, 500000 }; // testes definidos pelo trabalho

    for (size_t i = 0; i < sizeof(tamanhos)/sizeof(tamanhos[0]); i++) { // para cada tamanho
        size_t n = tamanhos[i];

        int *dados = NULL;
        size_t lidos = 0;
        TipoEntrada tipo = ENTRADA_ALEATORIA;

        if (carregar_entrada_padrao(n, &dados, &lidos, &tipo) != 0) { // valida a entrada
            fprintf(stderr, "Nao achei arquivo com pelo menos %zu numeros.\n", n);
            continue;
        }

        // mede todos os algoritmos
        medir_todos_algoritmos(dados, (int)n, tipo);

        

        free(dados);
    }

    return 0;
}