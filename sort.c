#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sort.h"
#include "dados.h"

// Conta uma comparação e uma movimentação
#define CMP(expr) (contar_comparacao(), (expr))
#define MOVIMENTA(dest, src)   \
    do {                      \
        contar_movimentacao(); \
        (dest) = (src);        \
    } while (0)

// Conta uma troca (1 troca)
#define TROCA(a, b)        \
    do {                   \
        contar_movimentacao(); \
        int tmp = (a);     \
        (a) = (b);         \
        (b) = tmp;         \
    } while (0)

// Bolha
void Bolha(int *v, int n) {
    if (!v || n <= 1) {
        return;
    }

    for (int i = n - 1; i >= 1; i--) {
        for (int j = 0; j < i; j++) {
            if (CMP(v[j] > v[j + 1])) {
                TROCA(v[j], v[j + 1]);
            }
        }
    }
}

// Bolha com critério de parada
void BolhaComCriterioDeParada(int *v, int n) {
    if (!v || n <= 1) {
        return;
    }

    int limite = n - 1;
    int houve_troca = 1;

    while (houve_troca && limite > 0) {
        houve_troca = 0;
        int novo_limite = 0;

        for (int j = 1; j <= limite; j++) {
            if (CMP(v[j - 1] > v[j])) {
                TROCA(v[j - 1], v[j]);
                houve_troca = 1;
                novo_limite = j;
            }
        }

        limite = novo_limite;
    }
}

// Inserção direta
void InsercaoDireta(int *v, int n) {
    if (!v || n <= 1) {
        return;
    }

    for (int i = 1; i < n; i++) {
        int chave = v[i];
        int j = i - 1;

        while (j >= 0 && CMP(chave < v[j])) {
            MOVIMENTA(v[j + 1], v[j]);
            j--;
        }

        MOVIMENTA(v[j + 1], chave);
    }
}

// Inserção binária
void InsercaoBinaria(int *v, int n) {
    if (!v || n <= 1) {
        return;
    }

    for (int i = 1; i < n; i++) {
        int chave = v[i];
        int esq = 0;
        int dir = i;

        while (esq < dir) {
            int meio = esq + (dir - esq) / 2;
            if (CMP(v[meio] <= chave)) {
                esq = meio + 1;
            } else {
                dir = meio;
            }
        }

        for (int j = i; j > dir; j--) {
            MOVIMENTA(v[j], v[j - 1]);
        }

        MOVIMENTA(v[dir], chave);
    }
}

// Inserção ternária
void InsercaoTernaria(int *v, int n) {
    if (!v || n <= 1) {
        return;
    }
    for (int i = 1; i < n; i++) {
        int chave = v[i];
        int esq = 0;
        int dir = i;

        while (esq < dir) {
            int terco1 = esq + (dir - esq) / 3;
            int terco2 = dir - (dir - esq) / 3;

            if (CMP(chave < v[terco1])) {
                dir = terco1;
            } else if (CMP(chave > v[terco2])) {
                esq = terco2 + 1;
            } else {
                esq = terco1 + 1;
                dir = terco2;
            }
        }

        for (int j = i; j > dir; j--) {
            MOVIMENTA(v[j], v[j - 1]);
        }

        MOVIMENTA(v[dir], chave);
    }
}

// Shell-sort
void ShellSort(int *v, int n) {
    if (!v || n <= 1) {
        return;
    }

    int h = 1;
    while (h < n) {
        h = 3 * h + 1;
    }

    while (h > 1) {
        h /= 3;
        for (int i = h; i < n; i++) {
            int aux = v[i];
            int j = i - h;
            while (j >= 0 && CMP(aux < v[j])) {
                MOVIMENTA(v[j + h], v[j]);
                j -= h;
            }
            MOVIMENTA(v[j + h], aux);
        }
    }
}

// Seleção direta
void SelecaoDireta(int *v, int n) {
    if (!v || n <= 1) {
        return;
    }

    for (int i = 0; i < n - 1; i++) {
        int menor = i;

        for (int j = i + 1; j < n; j++) {
            if (CMP(v[j] < v[menor])) {
                menor = j;
            }
        }

        if (menor != i) {
            TROCA(v[i], v[menor]);
        }
    }
}

// Heapsort
static void CriaHeap(int *v, int inicio, int fim) {
    int aux = v[inicio];
    int j = inicio * 2 + 1;

    while (j <= fim) {
        if (j < fim && CMP(v[j] < v[j + 1])) {
            j++;
        }

        if (CMP(aux < v[j])) {
            MOVIMENTA(v[inicio], v[j]);
            inicio = j;
            j = inicio * 2 + 1;
        } else {
            j = fim + 1;
        }
    }

    MOVIMENTA(v[inicio], aux);
}

void HeapSort(int *v, int n) {
    if (!v || n <= 1) {
        return;
    }

    for (int i = (n - 1) / 2; i >= 0; i--) {
        CriaHeap(v, i, n - 1);
    }

    for (int i = n - 1; i > 0; i--) {
        TROCA(v[0], v[i]);
        CriaHeap(v, 0, i - 1);
    }
}

// Quicksort (pivô centro)
void QuickSortCentro(int *v, int n) {
    if (!v || n <= 1) {
        return;
    }
    int esq = 0;
    int dir = n - 1;

    int i = esq;
    int j = dir;
    int pivo = v[(i + j) / 2];

    while (i <= j) {
        while (CMP(v[i] < pivo)) {
            i++;
        }
        while (CMP(v[j] > pivo)) {
            j--;
        }
        if (i <= j) {
            TROCA(v[i], v[j]);
            i++;
            j--;
        }
    }

    if (esq < j) {
        QuickSortCentro(v, j - esq + 1);
    }
    if (i < dir) {
        QuickSortCentro(v + i, dir - i + 1);
    }
}

// Quicksort (pivô fim)
void QuickSortFim(int *v, int n) {
    if (!v || n <= 1) return;

    int esq = 0;
    int dir = n - 1;

    while (esq < dir) {
        int i = esq;
        int j = dir;
        int pivo = v[dir]; // pivô no fim

        while (i <= j) {
            while (i < dir && CMP(v[i] < pivo)) i++;
            while (j > esq && CMP(v[j] > pivo)) j--;

            if (i <= j) {
                TROCA(v[i], v[j]);
                i++;
                j--;
            }
        }

        int tam_esq = j - esq + 1;
        int tam_dir = dir - i + 1;

        // Recursar só no menor lado
        if (tam_esq < tam_dir) {
            if (esq < j) QuickSortFim(v + esq, tam_esq);
            esq = i; 
        } else {
            if (i < dir) QuickSortFim(v + i, tam_dir);
            dir = j; 
        }
    }
}

static int MedianaDeTres(int a, int b, int c) {
    if ((CMP(a >= b) && CMP(a <= c)) || (CMP(a <= b) && CMP(a >= c))) {
        return a;
    }
    if ((CMP(b >= a) && CMP(b <= c)) || (CMP(b <= a) && CMP(b >= c))) {
        return b;
    }
    return c;
}

// Quicksort (pivô mediana)
void QuickSortMediana(int *v, int n) {
    if (!v || n <= 1) {
        return;
    }
    int esq = 0;
    int dir = n - 1;

    int i = esq;
    int j = dir;
    int pivo = MedianaDeTres(v[esq], v[(esq + dir) / 2], v[dir]);

    while (i <= j) {
        while (CMP(v[i] < pivo)) {
            i++;
        }
        while (CMP(v[j] > pivo)) {
            j--;
        }
        if (i <= j) {
            TROCA(v[i], v[j]);
            i++;
            j--;
        }
    }

    if (esq < j) {
        QuickSortMediana(v, j - esq + 1);
    }
    if (i < dir) {
        QuickSortMediana(v + i, dir - i + 1);
    }
}

void QuickSort(int *v, int n) {
    QuickSortCentro(v, n);
}

// Mergesort
static void Intercala(int *v, int inicio, int meio, int fim) {
    int tam = fim - inicio + 1;
    int *tmp = (int *)malloc((size_t)tam * sizeof(int));
    if (!tmp) {
        perror("falha ao alocar buffer de merge");
        return;
    }
    int i = inicio;
    int j = meio + 1;
    int k = 0;

    while (i <= meio || j <= fim) {
        if (i > meio) {
            MOVIMENTA(tmp[k++], v[j++]);
        } else if (j > fim) {
            MOVIMENTA(tmp[k++], v[i++]);
        } else if (CMP(v[i] <= v[j])) {
            MOVIMENTA(tmp[k++], v[i++]);
        } else {
            MOVIMENTA(tmp[k++], v[j++]);
        }
    }

    for (k = 0; k < tam; k++) {
        MOVIMENTA(v[inicio + k], tmp[k]);
    }

    free(tmp);
}

void MergeSort(int *v, int n) {
    if (!v || n <= 1) {
        return;
    }
    int inicio = 0;
    int fim = n - 1;
    int meio = (inicio + fim) / 2;

    MergeSort(v, meio + 1);
    MergeSort(v + meio + 1, fim - meio);
    Intercala(v, inicio, meio, fim);
}

// Radixsort (base 10)
void RadixSort(int *v, int n) {
    if (!v || n <= 1) {
        return;
    }

    int maior = v[0];
    for (int i = 1; i < n; i++) {
        if (CMP(v[i] > maior)) {
            maior = v[i];
        }
    }

    for (int exp = 1; maior / exp > 0; exp *= 10) {
        int cont[10];
        for (int i = 0; i < 10; i++) {
            cont[i] = 0;
        }

        for (int i = 0; i < n; i++) {
            int dig = (v[i] / exp) % 10;
            cont[dig] = cont[dig] + 1;
        }

        for (int i = 1; i < 10; i++) {
            cont[i] = cont[i] + cont[i - 1];
        }

        int *saida = (int *)malloc((size_t)n * sizeof(int));
        if (!saida) {
            perror("falha ao alocar vetor de saida do RadixSort");
            return;
        }

        for (int i = n - 1; i >= 0; i--) {
            int dig = (v[i] / exp) % 10;
            MOVIMENTA(saida[cont[dig] - 1], v[i]);
            cont[dig] = cont[dig] - 1;
        }

        for (int i = 0; i < n; i++) {
            MOVIMENTA(v[i], saida[i]);
        }

        free(saida);
    }
}

// Bucketsort (contagem simples)
void BucketSort(int *v, int n) {
    if (!v || n <= 1) {
        return;
    }

    int max = v[0];
    for (int i = 1; i < n; i++) {
        if (CMP(v[i] > max)) {
            max = v[i];
        }
    }

    int alcance = max + 1;
    int *cont = (int *)calloc((size_t)alcance, sizeof(int));
    if (!cont) {
        perror("falha ao alocar baldes");
        return;
    }

    for (int i = 0; i < n; i++) {
        cont[v[i]] = cont[v[i]] + 1;
    }

    int idx = 0;
    for (int valor = 0; valor < alcance; valor++) {
        while (CMP(cont[valor] > 0)) {
            MOVIMENTA(v[idx++], valor);
            cont[valor] = cont[valor] - 1;
        }
    }

    free(cont);
}
