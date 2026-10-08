

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using namespace std;

class Solution {
private:

    void intercalar(vector<string>& v, vector<string>& aux, int esq, int meio, int dir) {
        int i = esq;
        int j = meio + 1;
        int k = esq;

        while (i <= meio && j <= dir) {
            if (v[i].size() >= v[j].size()) {   // >= garante a estabilidade
                aux[k++] = v[i++];
            } else {
                aux[k++] = v[j++];
            }
        }
        while (i <= meio) aux[k++] = v[i++];
        while (j <= dir)  aux[k++] = v[j++];

        for (int p = esq; p <= dir; p++) {
            v[p] = aux[p];
        }
    }

    void mergeSort(vector<string>& v, vector<string>& aux, int esq, int dir) {
        if (esq >= dir) return;
        int meio = esq + (dir - esq) / 2;
        mergeSort(v, aux, esq, meio);
        mergeSort(v, aux, meio + 1, dir);
        intercalar(v, aux, esq, meio, dir);
    }

    int particionarLomuto(vector<string>& v, int esq, int dir) {
        int meio = esq + (dir - esq) / 2;
        swap(v[meio], v[dir]);                  // pivo vai para o fim
        size_t tamPivo = v[dir].size();

        int i = esq;                            // ponteiro de escrita
        for (int j = esq; j < dir; j++) {       // ponteiro de leitura
            if (v[j].size() > tamPivo) {        // decrescente por tamanho
                swap(v[i], v[j]);
                i++;
            }
        }
        swap(v[i], v[dir]);                     // pivo vai para a posicao final
        return i;
    }

    void quickSort(vector<string>& v, int esq, int dir) {
        if (esq >= dir) return;
        int p = particionarLomuto(v, esq, dir);
        quickSort(v, esq, p - 1);
        quickSort(v, p + 1, dir);
    }

public:
    // a) Merge Sort estavel
    vector<string> mergeSortByLength(vector<string> palavras) {
        int n = palavras.size();
        vector<string> aux(n);
        mergeSort(palavras, aux, 0, n - 1);
        return palavras;
    }

    // b) Quick Sort (Lomuto) instavel
    vector<string> quickSortByLength(vector<string> palavras) {
        quickSort(palavras, 0, (int)palavras.size() - 1);
        return palavras;
    }
};

static void imprimirLinha(const string& prefixo, const vector<string>& palavras) {
    cout << prefixo;
    for (const string& palavra : palavras) {
        cout << ' ' << palavra;
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<string> palavras(n);
    for (int i = 0; i < n; i++) {
        cin >> palavras[i];
    }

    Solution solucao;
    imprimirLinha("[MergeSort]", solucao.mergeSortByLength(palavras));
    imprimirLinha("[QuickSort]", solucao.quickSortByLength(palavras));

    return 0;
}