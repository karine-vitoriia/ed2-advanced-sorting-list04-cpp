

#include <iostream>
#include <vector>
#include <utility>

using namespace std;

using ll = long long;

class Solution {
private:

    ll mergeAndCount(vector<int>& v, vector<int>& aux, int esq, int meio, int dir) {
        int i = esq;        // ponteiro da metade esquerda
        int j = meio + 1;   // ponteiro da metade direita
        int k = esq;        // posicao de escrita no vetor auxiliar
        ll inversoes = 0;

        while (i <= meio && j <= dir) {
            if (v[i] <= v[j]) {
                aux[k++] = v[i++];
            } else {
                int tamEsq = meio - i + 1;
                inversoes += tamEsq;
                aux[k++] = v[j++];
            }
        }

        while (i <= meio) aux[k++] = v[i++];
        while (j <= dir)  aux[k++] = v[j++];

        for (int p = esq; p <= dir; p++) {
            v[p] = aux[p];
        }
        return inversoes;
    }

    ll mergeSort(vector<int>& v, vector<int>& aux, int esq, int dir) {
        if (esq >= dir) return 0;

        int meio = esq + (dir - esq) / 2;

        ll inversoesEsq   = mergeSort(v, aux, esq, meio);
        ll inversoesDir   = mergeSort(v, aux, meio + 1, dir);
        ll inversoesCruz  = mergeAndCount(v, aux, esq, meio, dir);

        return inversoesEsq + inversoesDir + inversoesCruz;
    }

public:
    // Ordena nums em ordem crescente e devolve {vetor ordenado, total de inversoes}
    pair<vector<int>, ll> mergeSortCountInversions(vector<int> nums) {
        int n = nums.size();
        vector<int> aux(n);
        ll inversoes = mergeSort(nums, aux, 0, n - 1);
        return {nums, inversoes};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> nums(n);
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
    }

    Solution solucao;
    pair<vector<int>, ll> resultado = solucao.mergeSortCountInversions(nums);

    for (int i = 0; i < n; i++) {
        if (i > 0) cout << ' ';
        cout << resultado.first[i];
    }
    cout << '\n' << resultado.second << '\n';

    return 0;
}