#include <iostream>
#include <vector>
#include <utility>

using namespace std;

class Solution {
private:

    int particionarParidadeHoare(vector<int>& v) {
        int esquerda = 0;
        int direita = (int)v.size() - 1;

        while (true) {
            while (esquerda <= direita && v[esquerda] % 2 == 0) esquerda++;  // acha um impar
            while (esquerda <= direita && v[direita] % 2 != 0) direita--;    // acha um par

            if (esquerda >= direita) break;

            swap(v[esquerda], v[direita]);
            esquerda++;
            direita--;
        }
        return esquerda;
    }


    // crescente = true ordena de forma crescente; false, decrescente.
    void quickSortHoare(vector<int>& v, int esq, int dir, bool crescente) {
        if (esq >= dir) return;

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
    }

public:
    vector<int> paritySortHoare(vector<int> nums) {
        int n = nums.size();
        int qtdPares = particionarParidadeHoare(nums);

        quickSortHoare(nums, 0, qtdPares - 1, true);   // pares: crescente
        quickSortHoare(nums, qtdPares, n - 1, false);  // Ã­mpares: decrescente

        return nums;
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
    vector<int> resultado = solucao.paritySortHoare(nums);

    for (int i = 0; i < n; i++) {
        if (i > 0) cout << ' ';
        cout << resultado[i];
    }
    cout << '\n';

    return 0;
}