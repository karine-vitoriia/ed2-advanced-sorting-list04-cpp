# ed2-advanced-sorting-list04-cpp

Lista de Exercícios 04 de **Estrutura de Dados II** (IFTM, Campus Patrocínio): **Merge Sort** e **Quick Sort**, com os particionamentos de **Lomuto** e de **Hoare**.

Este README tem duas partes:

- **O raciocínio do código:** como cada questão foi pensada, o trecho que faz o trabalho e um passo a passo com os exemplos do PDF.
- **Como usar o projeto:** o que instalar, como compilar, rodar e testar.

Os códigos estão em C++17 e seguem o formato do LeetCode: uma `class Solution` com o método que resolve o problema e um `main()` que lê a entrada digitada no console. Os nomes de variáveis estão em português (`esquerda`, `tamEsq`, `inversoes` etc.) e os nomes de classe e de método ficam em inglês.

## Sumário

1. [Visão geral do projeto](#1-visão-geral-do-projeto)
2. [O raciocínio de cada questão](#2-o-raciocínio-de-cada-questão)
   - [Palavras que aparecem muito](#palavras-que-aparecem-muito)
   - [Questão 1: Merge Sort contando inversões](#questão-1-merge-sort-contando-inversões)
   - [Questão 2: estabilidade (Merge Sort contra Quick Sort)](#questão-2-estabilidade-merge-sort-contra-quick-sort)
   - [Questão 3: Quickselect com Lomuto](#questão-3-quickselect-com-lomuto)
   - [Questão 4: ordenação par-ímpar com Hoare](#questão-4-ordenação-par-ímpar-com-hoare)
   - [Lomuto contra Hoare](#lomuto-contra-hoare)
   - [Resumo das complexidades](#resumo-das-complexidades)
3. [O que você precisa instalar](#3-o-que-você-precisa-instalar)
4. [Como compilar e rodar](#4-como-compilar-e-rodar)
5. [Como digitar a entrada de cada questão](#5-como-digitar-a-entrada-de-cada-questão)
6. [Como testar](#6-como-testar)
7. [Problemas comuns](#7-problemas-comuns)


---

## 1. Visão geral do projeto

```
ed2-advanced-sorting-list04-cpp/
|-- src/
|   |-- Exer01_MergeSortCountInversions.cpp
|   |-- Exer02_StabilityMergeSortVsQuickSort.cpp
|   |-- Exer03_QuickselectLomutoKthLargest.cpp
|   `-- Exer04_HoarePartitionParitySort.cpp
|-- .gitignore
|-- CMakeLists.txt
|-- main.cpp
`-- README.md
```

| # | Arquivo (`src/`) | Problema | Técnica | Complexidade |
|---|---|---|---|---|
| 1 | `Exer01_...cpp` | Contar inversões e ordenar | Merge Sort | O(N log N) |
| 2 | `Exer02_...cpp` | Mostrar por que o Quick Sort não é estável | Merge Sort contra Quick Sort (Lomuto) | O(N log N) |
| 3 | `Exer03_...cpp` | K-ésimo maior elemento, contando trocas | Quickselect com Lomuto | O(N) médio |
| 4 | `Exer04_...cpp` | Pares antes dos ímpares, cada grupo ordenado | Partição de Hoare | O(N) na partição |

Outros arquivos:

| Arquivo | O que faz |
|---|---|
| `CMakeLists.txt` | Diz ao CMake como compilar. Cria **um programa para cada questão** |
| `main.cpp` | Só mostra a lista das questões. Não resolve nada |
| `.gitignore` | Evita enviar ao Git as pastas de compilação (`build/`, `cmake-build-*/`) e a pasta `.idea/` |

> **Por que cada questão é um programa separado?**
> Cada arquivo da pasta `src/` tem o seu próprio `main()`. O C++ não aceita dois `main()` no mesmo programa, então o CMake monta quatro programas independentes (mais o `main` da raiz).

---

## 2. O raciocínio de cada questão

### Palavras que aparecem muito

| Palavra | O que significa |
|---|---|
| **Divisão e conquista** | Quebrar o problema em partes menores, resolver cada parte e juntar os resultados. Merge Sort e Quick Sort fazem isso |
| **Pivô** | Um elemento escolhido como referência. Os menores vão para um lado e os maiores para o outro |
| **Partição** | O passo que separa o vetor em volta do pivô |
| **Estável** | Um algoritmo é estável quando elementos com a mesma chave ficam na ordem em que entraram |
| **Troca (swap)** | Trocar de lugar dois elementos do vetor |

---

### Questão 1: Merge Sort contando inversões

**O problema.** Uma *inversão* é um par de posições `(i, j)` com `i < j` e `A[i] > A[j]`. Contar um a um custa O(N²). Dá para contar durante o Merge Sort em O(N log N).

**A ideia.** Divida o vetor ao meio, resolva cada metade e conte as inversões que "cruzam" as duas metades na hora de juntá-las. O total é a soma das três partes:

```cpp
ll inversoesEsq  = mergeSort(v, aux, esq, meio);          // inversões só dentro da esquerda
ll inversoesDir  = mergeSort(v, aux, meio + 1, dir);      // inversões só dentro da direita
ll inversoesCruz = mergeAndCount(v, aux, esq, meio, dir); // inversões entre as duas metades

return inversoesEsq + inversoesDir + inversoesCruz;
```

**O trecho que conta** (dentro da intercalação, `mergeAndCount`):

```cpp
while (i <= meio && j <= dir) {
    if (v[i] <= v[j]) {
        aux[k++] = v[i++];
    } else {
        int tamEsq = meio - i + 1;   // quantos ainda restam na esquerda
        inversoes += tamEsq;
        aux[k++] = v[j++];
    }
}
```

**Por que `+= tamEsq` está certo.** Na hora de juntar, as duas metades já estão ordenadas. Se `v[j]` (da direita) é menor que `v[i]` (da esquerda), então `v[j]` também é menor que **todos** os que ainda restam na esquerda, de `v[i]` até `v[meio]`. Esses `tamEsq` pares são inversões. Uma única soma conta todos, sem comparar par por par. É isso que baixa o custo de O(N²) para O(N log N).

**Cada inversão é contada uma única vez.** Um par `(a, b)`, com `a` antes de `b`, só fica separado em metades diferentes em uma única intercalação, a que junta a metade onde está `a` com a metade onde está `b`. Os pares que ficam dentro da mesma metade já foram contados na recursão.

**Passo a passo com `[2, 3, 8, 6, 1]`:**

| Intercalação | O que acontece | Inversões |
|---|---|---|
| `[2]` com `[3]`, depois `[2,3]` com `[8]` | Nada sai fora de ordem | 0 |
| `[6]` com `[1]` | O 1 sai antes do 6 (`tamEsq = 1`) | +1, o par (6, 1) |
| `[2,3,8]` com `[1,6]` | O 1 sai antes do 2 (`tamEsq = 3`) | +3, os pares (2,1), (3,1), (8,1) |
| (mesma) | O 6 sai antes do 8 (`tamEsq = 1`) | +1, o par (8, 6) |

Total: 0 + 1 + 3 + 1 = **5 inversões**, e o vetor sai ordenado: `1 2 3 6 8`. É o mesmo resultado do enunciado.

**Detalhes do código:**

- O contador é `long long` (`ll`). Com N = 100.000, o número de inversões pode passar de 4 bilhões, e o `int` só vai até cerca de 2 bilhões.
- O vetor `aux` é criado **uma vez só** e passado por referência. Assim não se aloca memória a cada chamada da recursão.
- `meio = esq + (dir - esq) / 2` dá o mesmo resultado de `(esq + dir) / 2`, mas não estoura o `int` com valores grandes.
- No empate, `v[i] <= v[j]` tira primeiro o da esquerda. Nesta questão os números são todos diferentes, então o empate nem acontece, mas esse é o hábito certo para um Merge Sort.

**Complexidade:** O(N log N) de tempo e O(N) de memória auxiliar.

---

### Questão 2: estabilidade (Merge Sort contra Quick Sort)

**O problema.** Ordenar palavras pelo **tamanho, do maior para o menor**. Se duas palavras têm o mesmo tamanho, a que veio primeiro na entrada deve continuar na frente. Isso é *estabilidade*. O exercício roda os dois algoritmos sobre a mesma entrada para mostrar que o Merge Sort mantém essa regra e o Quick Sort não.

#### a) Merge Sort (estável)

```cpp
while (i <= meio && j <= dir) {
    if (v[i].size() >= v[j].size()) {   // >= garante a estabilidade
        aux[k++] = v[i++];
    } else {
        aux[k++] = v[j++];
    }
}
```

O `>=` é o ponto principal. Se os tamanhos são iguais, sai primeiro o elemento da **esquerda**, que já estava antes na entrada. Se o código usasse `>`, no empate sairia o da direita primeiro e a ordem se inverteria.

#### b) Quick Sort com Lomuto (instável)

```cpp
int particionarLomuto(vector<string>& v, int esq, int dir) {
    int meio = esq + (dir - esq) / 2;
    swap(v[meio], v[dir]);                  // pivô vai para o fim
    size_t tamPivo = v[dir].size();

    int i = esq;                            // ponteiro de escrita
    for (int j = esq; j < dir; j++) {       // ponteiro de leitura
        if (v[j].size() > tamPivo) {        // decrescente por tamanho
            swap(v[i], v[j]);
            i++;
        }
    }
    swap(v[i], v[dir]);                     // pivô vai para a posição final
    return i;
}
```

Como funciona: o pivô é levado para o fim. O ponteiro `j` lê o vetor e, quando acha uma palavra **maior** que o pivô, troca com a posição `i` e avança `i`. No fim, o pivô é trocado com a posição `i`, que é o lugar definitivo dele.

**Passo a passo com `[mar, Coder, sol, broad]`:**

| Passo | Vetor |
|---|---|
| Começo | `[mar, Coder, sol, broad]` |
| O pivô do meio (`Coder`) vai para o fim | `[mar, broad, sol, Coder]` |
| Nenhuma palavra é maior que 5 letras, então `i` fica em 0 | `[mar, broad, sol, Coder]` |
| Swap final do pivô: `swap(v[0], v[3])` | `[Coder, broad, sol, mar]` |
| Trecho `[broad, sol, mar]` sai do mesmo jeito | `[Coder, broad, sol, mar]` |

Veja o `mar`. Ele estava na posição 0, antes do `sol`, e o swap do pivô o mandou para a posição 3, **por cima do `sol`**. Resultado: `Coder broad sol mar`. Com o Merge Sort o resultado é `Coder broad mar sol`, com `mar` antes de `sol`, como na entrada.

> **Por que pivô do meio e `>` estrito?** Entre as variações testadas (Lomuto e Hoare, com o pivô no começo, no meio e no fim), essa é a única que reproduz a saída dos **dois** exemplos do PDF. Veja a [seção 7](#7-notas-sobre-os-exemplos-do-pdf).

#### c) Análise teórica: por que um estraga e o outro mantém

**Quick Sort (trabalha no próprio vetor, só com trocas):**

- Um `swap(v[i], v[j])` leva um elemento de `i` para `j`, e entre eles pode haver elementos de mesma chave. Ele "pula por cima" deles e a ordem deles muda. É a *troca de longa distância*.
- O swap final do pivô repete o problema: o elemento da posição `i` vai para o fim do trecho e atravessa todos que estavam no meio.
- O swap só enxerga a chave (o tamanho). Nada no algoritmo guarda a posição original, então ele não sabe que dois empatados deveriam ficar na mesma ordem.

**Merge Sort (usa um vetor auxiliar e só copia):**

- Os ponteiros `i` e `j` só andam para frente, e cada elemento é **copiado** para `aux[k]`, com `k` sempre crescendo. Nenhum elemento pula sobre outro da mesma metade.
- No empate, o `>=` copia primeiro o da esquerda. Todo elemento da metade esquerda estava, na entrada, antes de qualquer elemento da direita. Então a ordem original é respeitada.
- Por indução: pedaços de 1 elemento são estáveis, e juntar duas metades estáveis com `>=` dá um resultado estável. Logo, o vetor inteiro é estável.

**O preço da estabilidade:** o Merge Sort gasta O(N) de memória extra. O Quick Sort gasta O(1) (fora a pilha da recursão). Ele economiza memória e costuma ser mais rápido na prática, mas abre mão da ordem dos empates.

**Complexidade:** Merge Sort em O(N log N) sempre. Quick Sort em O(N log N) no caso médio e O(N²) no pior caso.

---

### Questão 3: Quickselect com Lomuto

**O problema.** Achar o K-ésimo **maior** elemento sem ordenar o vetor inteiro, e contar quantas trocas foram feitas.

**A ideia.** Em um vetor ordenado do menor para o maior, o K-ésimo maior fica na posição `alvo = n - K`. Faça uma partição e veja em que posição `p` o pivô parou. O pivô já está no lugar final dele, então dá para decidir para que lado ir:

```cpp
int alvo = n - k;

while (esq <= dir) {
    int p = particionarLomuto(nums, esq, dir, trocas);

    if (p == alvo) {
        return {nums[p], nums, trocas};   // achou
    } else if (p < alvo) {
        esq = p + 1;                      // o alvo está à direita: descarta a esquerda
    } else {
        dir = p - 1;                      // o alvo está à esquerda: descarta a direita
    }
}
```

**A partição de Lomuto e a contagem de trocas:**

```cpp
void trocar(vector<int>& v, int a, int b, ll& trocas) {
    if (a == b) return;      // auto-troca não muda nada: não conta
    swap(v[a], v[b]);
    trocas++;
}

int particionarLomuto(vector<int>& v, int esq, int dir, ll& trocas) {
    int pivo = v[dir];                      // pivô = último elemento
    int i = esq;                            // ponteiro de escrita
    for (int j = esq; j < dir; j++) {       // ponteiro de leitura
        if (v[j] <= pivo) {                 // menores ou iguais vão para a esquerda
            trocar(v, i, j, trocas);
            i++;
        }
    }
    trocar(v, i, dir, trocas);              // pivô vai para a posição final
    return i;
}
```

Toda troca passa pela função `trocar`. Ela ignora as auto-trocas (`a == b`), como o enunciado pede.

**Passo a passo com `[3, 2, 1, 5, 6, 4]` e K = 2 (então `alvo = 4`):**

| Rodada | Trecho | Pivô | O que acontece | Vetor depois | Trocas |
|---|---|---|---|---|---|
| 1 | `[0..5]` | 4 | 3, 2 e 1 já estão na posição certa (auto-trocas, ignoradas). O swap final troca 5 e 4. Pivô em `p = 3`, menor que 4: vai para a direita | `[3,2,1,4,6,5]` | 1 |
| 2 | `[4..5]` | 5 | O 6 não é `<=` 5. O swap final troca 6 e 5. Pivô em `p = 4`, igual ao alvo | `[3,2,1,4,5,6]` | 2 |

Resposta: **5**, com 2 trocas e vetor final `3 2 1 4 5 6`. É a saída do enunciado.

**O vetor final não está ordenado.** O Quickselect só garante que tudo à esquerda de `p` é `<=` ao pivô e tudo à direita é maior. Por isso o enunciado pede para imprimir o estado do vetor: ele mostra até onde o algoritmo foi.

**Detalhes do código:**

- A versão é **iterativa** (um `while`) e não recursiva. Com N = 100.000 e má sorte, a recursão poderia empilhar até N chamadas e estourar a pilha.
- O custo médio é O(N): cada rodada trabalha em cerca de metade do trecho da anterior, e N + N/2 + N/4 + ... dá aproximadamente 2N.
- No exemplo 2 do PDF (veja a [seção 7](#7-notas-sobre-os-exemplos-do-pdf)), os pivôs 6, 5, 5 e 4 já estão nas posições finais, então o resultado é 0 trocas.

**Complexidade:** O(N) médio, O(N²) no pior caso (por exemplo, vetor já ordenado, com o pivô sempre no último elemento). Memória extra O(1).

---

### Questão 4: ordenação par-ímpar com Hoare

**O problema.** Deixar todos os pares antes dos ímpares, com os pares em ordem crescente e os ímpares em ordem decrescente.

**A ideia.** Duas etapas:

1. **Separar** pares e ímpares com a partição de Hoare.
2. **Ordenar** cada grupo, com um Quick Sort que também usa a partição de Hoare.

```cpp
int qtdPares = particionarParidadeHoare(nums);

quickSortHoare(nums, 0, qtdPares - 1, true);   // pares: crescente
quickSortHoare(nums, qtdPares, n - 1, false);  // ímpares: decrescente
```

#### Etapa 1: separar com dois ponteiros que se encontram

```cpp
while (true) {
    while (esquerda <= direita && v[esquerda] % 2 == 0) esquerda++;  // pula pares: acha um ímpar
    while (esquerda <= direita && v[direita] % 2 != 0) direita--;    // pula ímpares: acha um par

    if (esquerda >= direita) break;

    swap(v[esquerda], v[direita]);   // um ímpar à esquerda e um par à direita: troca
    esquerda++;
    direita--;
}
return esquerda;   // aqui começam os ímpares (= quantidade de pares)
```

O ponteiro `esquerda` parte do começo e anda enquanto o número é par (já está no lado certo). O ponteiro `direita` parte do fim e anda enquanto o número é ímpar. Quando os dois param, `esquerda` está em um ímpar que deveria ir para a direita e `direita` está em um par que deveria ir para a esquerda. Troca-se os dois, e **cada troca conserta dois elementos de uma vez**. Quando os ponteiros se cruzam, termina.

Esse é o ganho do Hoare: ele só troca quando os dois lados estão errados. Aqui não existe valor de pivô. O "pivô" é a pergunta "é par?".

**Passo a passo com `[4, 3, 2, 7, 8, 1]`:**

| Passo | O que acontece | Vetor |
|---|---|---|
| 1 | `esquerda` pula o 4 e para no 3. `direita` pula o 1 e para no 8 | `[4, 3, 2, 7, 8, 1]` |
| 2 | Troca o 3 com o 8 (posições 1 e 4). **Única troca** | `[4, 8, 2, 7, 3, 1]` |
| 3 | `esquerda` pula o 2 e para no 7. `direita` anda até passar por `esquerda`. Os ponteiros se cruzaram, então termina e devolve 3 | `[4, 8, 2 \| 7, 3, 1]` |

Os 3 primeiros são pares e os 3 últimos são ímpares. Só 1 troca foi necessária.

#### Etapa 2: ordenar cada grupo

```cpp
int pivo = v[esq + (dir - esq) / 2];
int i = esq - 1;
int j = dir + 1;

while (true) {
    do { i++; } while (crescente ? v[i] < pivo : v[i] > pivo);
    do { j--; } while (crescente ? v[j] > pivo : v[j] < pivo);

    if (i >= j) break;
    swap(v[i], v[j]);
}

quickSortHoare(v, esq, j, crescente);
quickSortHoare(v, j + 1, dir, crescente);
```

- O parâmetro `crescente` inverte as comparações. Uma função só serve para os pares (crescente) e para os ímpares (decrescente).
- O pivô é o elemento **do meio**, com divisão inteira para baixo. Com o pivô no último elemento, o Hoare clássico pode devolver `j == dir` e a recursão nunca diminuiria.
- A recursão é em `(esq, j)` e `(j + 1, dir)`. Usar `(esq, j - 1)`, como no Lomuto, estaria errado.
- Os ponteiros param nos elementos iguais ao pivô e os trocam. Isso mantém as partições equilibradas mesmo com muitos números repetidos.

Para `[4,8,2]` crescente sai `[2,4,8]`, e para `[7,3,1]` decrescente continua `[7,3,1]`. Resultado: `2 4 8 7 3 1`.

**Casos de borda.** Se todos são pares, `esquerda` vai até o fim e o grupo dos ímpares fica vazio. Se todos são ímpares, `qtdPares` é 0 e o grupo dos pares fica vazio. Nos dois casos o Quick Sort recebe um trecho vazio e volta na hora.

**Complexidade:** a separação é O(N) de tempo e O(1) de memória. Cada ordenação é O(N log N) no caso médio.

---

### Lomuto contra Hoare

As Questões 3 e 4 usam um tipo de partição cada uma, e a escolha tem motivo:

| | Lomuto | Hoare |
|---|---|---|
| Ponteiros | Os dois andam no mesmo sentido (leitura e escrita) | Partem das pontas e se encontram |
| Onde o pivô termina | **Na posição final exata** | Não necessariamente |
| Número de trocas | Maior | Menor |
| Fica melhor em | Quickselect, que compara `p` com `alvo` | Separar por uma condição, como par ou ímpar |

A Questão 3 usa **Lomuto** porque o pivô cai na posição certa, e é isso que permite comparar `p` com `alvo` e descartar um lado. A Questão 4 usa **Hoare** porque o que importa é fazer poucas trocas, e cada troca arruma dois elementos.

---

### Resumo das complexidades

| # | Tempo | Memória extra | Observação |
|---|---|---|---|
| 1 | O(N log N) | O(N) | O vetor `aux` do Merge Sort |
| 2 | Merge Sort: O(N log N). Quick Sort: O(N log N) médio, O(N²) no pior caso | Merge Sort: O(N). Quick Sort: O(1) | A estabilidade custa memória |
| 3 | O(N) médio, O(N²) no pior caso | O(1) | Pivô fixo no último elemento |
| 4 | Separar: O(N). Ordenar: O(N log N) médio | O(1) | Fora a pilha da recursão |

---

## 3. O que você precisa instalar

Você precisa de **uma** das opções abaixo.

| Opção | O que instalar |
|---|---|
| **A. CLion** (mais fácil) | Só o CLion. Ele já vem com CMake e, na maioria dos casos, com um compilador |
| **B. Terminal** | Um compilador C++ que aceite **C++17** (`g++` 7 ou mais novo, ou `clang++`) e o **CMake** 3.10 ou mais novo |

Para conferir no terminal se está tudo instalado:

```bash
g++ --version
cmake --version
```

Se os dois comandos mostrarem uma versão, está pronto. Se aparecer `command not found`, instale o programa que faltou.

---

## 4. Como compilar e rodar

### Opção A: pelo CLion (recomendado)

1. Abra o CLion e vá em **File > Open**.
2. Escolha a pasta `ed2-advanced-sorting-list04-cpp` (a que contém o `CMakeLists.txt`) e confirme.
3. Se o CLion perguntar se confia no projeto, clique em **Trust Project**.
4. Espere o CMake carregar. Isso aparece como uma barra de progresso no canto inferior. Quando terminar, a aba **CMake** fica sem erros.
5. No canto superior direito há uma caixa para escolher o que executar. Abra essa caixa e escolha **uma questão**:
   - `Exer01_MergeSortCountInversions`
   - `Exer02_StabilityMergeSortVsQuickSort`
   - `Exer03_QuickselectLomutoKthLargest`
   - `Exer04_HoarePartitionParitySort`
6. Clique no botão verde **Run** (ou aperte `Shift + F10`; no macOS, `Ctrl + R`).
7. O CLion compila e abre a janela **Run** embaixo. O programa fica **parado esperando você digitar**.
8. Clique dentro da janela **Run**, digite a entrada (veja a [seção 5](#5-como-digitar-a-entrada-de-cada-questão)) e aperte **Enter** no fim de cada linha.
9. O resultado aparece na hora. No final, o CLion mostra `Process finished with exit code 0`. Isso quer dizer que o programa terminou sem erro.

Para testar outro exemplo, clique em **Run** de novo.

**Dica: repetir sempre o mesmo exemplo sem digitar**

1. Crie um arquivo de texto (por exemplo `entrada.txt`) com a entrada da questão.
2. Vá em **Run > Edit Configurations** e escolha a questão.
3. Marque a opção **Redirect input from** (nas versões novas do CLion ela fica em **Modify options**) e escolha o `entrada.txt`.
4. Ao clicar em **Run**, o CLion usa o arquivo no lugar do teclado.

### Opção B: pelo terminal, com CMake

Dentro da pasta do projeto, rode:

```bash
cmake -S . -B build
cmake --build build
```

- O primeiro comando prepara a compilação na pasta `build/`.
- O segundo compila de verdade. Ele gera os cinco programas dentro de `build/`.

Para rodar uma questão, chame o programa dela e depois digite a entrada:

```bash
./build/Exer01_MergeSortCountInversions
./build/Exer02_StabilityMergeSortVsQuickSort
./build/Exer03_QuickselectLomutoKthLargest
./build/Exer04_HoarePartitionParitySort
```

> **Windows:** o programa termina em `.exe` e as barras são invertidas, por exemplo `build\Exer01_MergeSortCountInversions.exe`. Se você usa o Visual Studio como compilador, o programa fica em `build\Debug\`.

### Opção C: pelo terminal, só com g++ (sem CMake)

Serve para compilar uma questão rapidamente. Rode estes comandos dentro da pasta do projeto:

```bash
mkdir -p build
g++ -std=c++17 -Wall -Wextra src/Exer01_MergeSortCountInversions.cpp      -o build/Exer01_MergeSortCountInversions
g++ -std=c++17 -Wall -Wextra src/Exer02_StabilityMergeSortVsQuickSort.cpp -o build/Exer02_StabilityMergeSortVsQuickSort
g++ -std=c++17 -Wall -Wextra src/Exer03_QuickselectLomutoKthLargest.cpp   -o build/Exer03_QuickselectLomutoKthLargest
g++ -std=c++17 -Wall -Wextra src/Exer04_HoarePartitionParitySort.cpp      -o build/Exer04_HoarePartitionParitySort
```

Os programas saem com os **mesmos nomes** da Opção B, então os comandos de teste da [seção 6](#6-como-testar) servem para as duas.

> O `-std=c++17` é obrigatório. Sem ele, a Questão 3 não compila.

---

## 5. Como digitar a entrada de cada questão

Digite exatamente neste formato, com os números separados por **espaço** e **uma linha** para cada linha descrita abaixo.

| Questão | 1ª linha | 2ª linha |
|---|---|---|
| **1** | `N` (quantos números) | os `N` números, todos diferentes |
| **2** | `N` (quantas palavras) | as `N` palavras |
| **3** | `N K` (quantos números e qual K-ésimo maior) | os `N` números |
| **4** | `N` (quantos números) | os `N` números inteiros positivos |

O programa lê só o que precisa. Depois da segunda linha ele responde sozinho, sem você apertar `Ctrl + D`.

**Exemplo (Questão 1):**

```
5
2 3 8 6 1
```

---

## 6. Como testar

Cada questão do PDF traz exemplos com a saída esperada. Para testar, você digita a entrada do exemplo e confere se a saída é **igual** à esperada, linha por linha.

### 6.1 Testando pelo CLion

1. Escolha a questão na caixa do canto superior direito e clique em **Run**.
2. Digite a entrada do exemplo (os blocos da seção 6.2 mostram cada entrada).
3. Compare o que apareceu com a saída esperada.

### 6.2 Testando pelo terminal (Linux, macOS ou Git Bash no Windows)

O comando `printf` envia a entrada para o programa sem você digitar. Rode os comandos depois de compilar (Opção B ou C). Os blocos abaixo trazem o comando e a saída esperada.

> No `cmd` e no PowerShell do Windows o `printf` não existe. Lá, use o CLion, o Git Bash ou a dica do `Redirect input from`.

**Questão 1: contagem de inversões**

```bash
printf '5\n2 3 8 6 1\n' | ./build/Exer01_MergeSortCountInversions
```

```
1 2 3 6 8
5
```

```bash
printf '4\n1 2 3 4\n' | ./build/Exer01_MergeSortCountInversions
```

```
1 2 3 4
0
```

**Questão 2: estabilidade (Merge Sort contra Quick Sort)**

```bash
printf '4\nmar Coder sol broad\n' | ./build/Exer02_StabilityMergeSortVsQuickSort
```

```
[MergeSort] Coder broad mar sol
[QuickSort] Coder broad sol mar
```

```bash
printf '5\nsol lua estrela mar ceu\n' | ./build/Exer02_StabilityMergeSortVsQuickSort
```

```
[MergeSort] estrela sol lua mar ceu
[QuickSort] estrela ceu mar lua sol
```

O que observar: no Merge Sort, palavras de mesmo tamanho ficam na ordem em que entraram. No Quick Sort, essa ordem se embaralha.

**Questão 3: K-ésimo maior com Quickselect**

```bash
printf '6 2\n3 2 1 5 6 4\n' | ./build/Exer03_QuickselectLomutoKthLargest
```

```
5
3 2 1 4 5 6
2
```

```bash
printf '9 4\n3 2 3 1 2 4 5 5 6\n' | ./build/Exer03_QuickselectLomutoKthLargest
```

```
4
3 2 3 1 2 4 5 5 6
0
```

> A última linha deste segundo teste é **0** e não 2 como no PDF. Veja a explicação na [seção 7](#7-notas-sobre-os-exemplos-do-pdf).

**Questão 4: ordenação par-ímpar com Hoare**

```bash
printf '6\n4 3 2 7 8 1\n' | ./build/Exer04_HoarePartitionParitySort
```

```
2 4 8 7 3 1
```

```bash
printf '5\n10 9 8 7 6\n' | ./build/Exer04_HoarePartitionParitySort
```

```
6 8 10 9 7
```

### 6.3 Teste com a entrada vinda de um arquivo

Se preferir, grave a entrada em um arquivo e use `<` para entregá-la ao programa:

```bash
printf '6 2\n3 2 1 5 6 4\n' > entrada.txt
./build/Exer03_QuickselectLomutoKthLargest < entrada.txt
```

### 6.4 Teste de velocidade (opcional, Linux)

Os enunciados aceitam até 100.000 números. Este teste gera 100.000 números diferentes e mede o tempo da Questão 1:

```bash
{ echo 100000; shuf -i 1-1000000000 -n 100000 | tr '\n' ' '; echo; } > grande.txt
time (./build/Exer01_MergeSortCountInversions < grande.txt | tail -n 1)
```

O resultado é um número grande (a quantidade de inversões) e o tempo fica abaixo de 1 segundo. Isso mostra que o Merge Sort resolve em O(N log N).

> Esse número passa de 2 bilhões. Por isso o contador de inversões usa `long long` e não `int`.

Você pode usar o mesmo `grande.txt` na Questão 4 (`./build/Exer04_HoarePartitionParitySort < grande.txt`). Para a Questão 3, troque a primeira linha do arquivo por `100000 50000`.

---

## 7. Problemas comuns

| O que aconteceu | Como resolver |
|---|---|
| O programa abre e **não mostra nada** | Ele está esperando a entrada. Clique na janela **Run** e digite os números |
| O CLion não mostra as questões na caixa de execução | Vá em **Tools > CMake > Reload CMake Project** e espere terminar |
| `cmake: command not found` | Instale o CMake, ou use o CLion (ele já traz um) ou a Opção C |
| Erro de compilação falando de `structured bindings` ou `C++17` | Faltou `-std=c++17` no comando do `g++`. Pelo CMake isso já está configurado |
| `Permission denied` ao rodar `./build/...` | Compile primeiro (Opção B ou C). Se persistir, rode `chmod +x build/*` |
| `No such file or directory` ao rodar `./build/...` | Você está fora da pasta do projeto. Use `cd ed2-advanced-sorting-list04-cpp` |
| A saída vem errada ou o programa trava | Confira a ordem da entrada: o **primeiro** número é sempre `N` (na Questão 3, `N K`) |
| O `printf` não funciona no Windows | Use o CLion, o Git Bash ou a dica do `Redirect input from` |
| Quero recompilar do zero | Apague a pasta `build/` e repita a Opção B ou C |
