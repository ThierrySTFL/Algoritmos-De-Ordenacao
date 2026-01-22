#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"

// le numeros inteiros um por linha
int ler_numeros(const char *caminho, int **dados, size_t *quantidade) {
    if (!caminho || !dados || !quantidade) return -1;

    *dados = NULL;
    *quantidade = 0;

    FILE *in = fopen(caminho, "r");
    if (!in) return -1;

    const size_t MAX = 500000; // maximo tamanho do teste, fica grande de mais ? talvez, mas nao consigo pensar em outro jeito
    int *vetor = malloc(MAX * sizeof(int));
    if (!vetor) { fclose(in); return -1; }

    size_t total = 0;
    int ret = 0;

    while (total < MAX && (ret = fscanf(in, "%d", &vetor[total])) == 1) {
        total++;
    }

    // se chegou no maximo, verifica se tem mais
    if (total == MAX) {
        int extra;
        ret = fscanf(in, "%d", &extra);
        if (ret == 1) { // tinha mais que MAX
            free(vetor);
            fclose(in);
            return -1;
        }
    }

    // se parou por token inválido (ret == 0), é erro de formato
    if (ret == 0) {
        free(vetor);
        fclose(in);
        return -1;
    }

    fclose(in);
    *dados = vetor;
    *quantidade = total;
    return 0;
}

// escreve os numeros e os dados no arquivo
int escrever_saida(const char *caminho, const int *dados, size_t quantidade, Estatisticas estatisticas) {
    if (caminho == NULL) {
        return -1;
    }

    FILE *out = fopen(caminho, "w");
    if (out == NULL) {
        perror("falha ao criar arquivo de saida");
        return -1;
    }

    // escreve os numeros
    for (size_t i = 0; i < quantidade; i++) {
        fprintf(out, "%d\n", dados[i]);
    }

    // escreve os dados
    fprintf(out, "comparacoes:%lld\n", estatisticas.comparacoes);
    fprintf(out, "movimentacoes:%lld\n", estatisticas.movimentacoes);
    fprintf(out, "tempo_ms:%.3f\n", estatisticas.tempo_ms);

    fclose(out);
    return 0;
}

// carrega a entrada padrão a,c,d com pelo menos n de tamanho
int carregar_entrada_padrao(size_t n, int **dados, size_t *lidos, TipoEntrada *tipo) {
    if (!dados || !lidos) return -1;

    *dados = NULL;
    *lidos = 0;
    if (tipo) *tipo = ENTRADA_ALEATORIA; // aleatorio como padrao

    // tenta varios arquivos padroes
    char fa[64], fc[64], fd[64], f_old[64];
    snprintf(fa, sizeof(fa), "entrada_a_%zu.txt", n);
    snprintf(fc, sizeof(fc), "entrada_c_%zu.txt", n);
    snprintf(fd, sizeof(fd), "entrada_d_%zu.txt", n);
    snprintf(f_old, sizeof(f_old), "entrada_%zu.txt", n);

    const char *candidatos[] = { fa, fc, fd, f_old, "entrada.txt" }; // tenta nessa ordem

    for (size_t i = 0; i < sizeof(candidatos)/sizeof(candidatos[0]); i++) {
        int *vet = NULL;
        size_t qtd = 0;

        if (ler_numeros(candidatos[i], &vet, &qtd) == 0) {
            if (qtd >= n) {      
                *dados = vet;
                *lidos = qtd;

                if (tipo) { // identifica o tipo, mais facil que verificar na leitura de dados
                    if (strcmp(candidatos[i], fa) == 0) *tipo = ENTRADA_ALEATORIA;
                    else if (strcmp(candidatos[i], fc) == 0) *tipo = ENTRADA_CRESCENTE;
                    else if (strcmp(candidatos[i], fd) == 0) *tipo = ENTRADA_DECRESCENTE;
                    else *tipo = ENTRADA_ALEATORIA;
                }

                return 0;
            }
            free(vet); // não tem n valores, libera
        }
    }

    return -1;
}