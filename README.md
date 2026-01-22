## Algoritmos Implementados
- Bolha
- Bolha com criterio de parada
- Insercao direta
- Insercao binaria
- Insercao ternaria
- ShellSort
- Selecao direta
- HeapSort
- QuickSort (pivo centro, fim, mediana)
- MergeSort
- RadixSort
- BucketSort

## Estrutura do Projeto
- `main.c`: programa principal que carrega as entradas e mede todos os algoritmos.
- `dados.c` / `dados.h`: contadores, medicao de tempo e impressao/gravacao de resultados.
- `sort.c` / `sort.h`: implementacoes dos algoritmos de ordenacao.
- `arquivo.c` / `arquivo.h`: leitura de entradas e escrita das saidas.
- `gera.c`: utilitario para gerar entradas (saida em `saida.txt`).
- `entrada_*.txt`: arquivos de entrada com numeros (ex.: `entrada_a_10000.txt`).
- `saida_*.txt`: arquivos gerados para cada algoritmo e tamanho.

## Pre-requisitos
- Compilador C (GCC ou Clang).
- Ambiente de linha de comando no sistema operacional.

## Como Compilar e Executar
### Linux (passo a passo)
1) Compile o programa principal:
```bash
gcc -O2 -Wall -Wextra -pedantic main.c dados.c arquivo.c sort.c -o ordenar
```
2) (Opcional) Compile o gerador de entradas:
```bash
gcc -O2 -Wall -Wextra -pedantic gera.c -o gera
```
3) (Opcional) Gere um arquivo de entrada e renomeie:
```bash
./gera -a 10000
mv saida.txt entrada_a_10000.txt
```
4) Execute:
```bash
./ordenar
```

### macOS (passo a passo)
1) Compile o programa principal:
```bash
cc -O2 -Wall -Wextra -pedantic main.c dados.c arquivo.c sort.c -o ordenar
```
2) (Opcional) Compile o gerador de entradas:
```bash
cc -O2 -Wall -Wextra -pedantic gera.c -o gera
```
3) (Opcional) Gere um arquivo de entrada e renomeie:
```bash
./gera -a 10000
mv saida.txt entrada_a_10000.txt
```
4) Execute:
```bash
./ordenar
```

### Windows (passo a passo)
1) Instale o MinGW-w64 e abra o terminal do MSYS2/MinGW.
2) Compile o programa principal:
```bash
gcc -O2 -Wall -Wextra -pedantic main.c dados.c arquivo.c sort.c -o ordenar.exe
```
3) (Opcional) Compile o gerador de entradas:
```bash
gcc -O2 -Wall -Wextra -pedantic gera.c -o gera.exe
```
4) (Opcional) Gere um arquivo de entrada e renomeie:
```bash
.\gera.exe -a 10000
ren saida.txt entrada_a_10000.txt
```
5) Execute:
```bash
.\ordenar.exe
```

Notas:
- O programa procura arquivos `entrada_a_N.txt`, `entrada_c_N.txt` ou `entrada_d_N.txt` para cada tamanho N (10000, 100000, 500000). Se nao encontrar, tenta `entrada_N.txt` e `entrada.txt`.
- As saidas sao gravadas como `saida_<Algoritmo>_N.txt` com os numeros ordenados e as estatisticas no final.

## Artigo Tecnico
Os algoritmos de ordenacao implementados podem ser comparados pelo custo de comparacoes, movimentacoes e tempo. Metodos quadratios (Bolha, Insercao, Selecao) tendem a degradar rapidamente com o aumento de N, sendo mais adequados para entradas pequenas ou quase ordenadas. Metodos O(n log n) (HeapSort, QuickSort, MergeSort, ShellSort) escalam melhor para grandes volumes, embora tenham diferentes comportamentos em casos especificos (por exemplo, QuickSort depende da escolha de pivo). Algoritmos lineares por digito/contagem (RadixSort e BucketSort) podem ser superiores quando os valores sao inteiros em um intervalo conhecido, mas exigem memoria extra e podem ter custo maior de inicializacao.

## Autor
Thierry Stofel e Lucas Tavares
