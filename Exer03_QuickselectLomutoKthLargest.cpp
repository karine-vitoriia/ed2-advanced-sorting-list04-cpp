
#include <iostream>
#include <vector>
#include <tuple>
#include <utility>

using namespace std;

using ll = long long;

class Solution {
private:

    void trocar(vector<int>& v, int a, int b, ll& trocas) {
        if (a == b) return;
        swap(v[a], v[b]);
        trocas++;
    }

    int particionarLomuto(vector<int>& v, int esq, int dir, ll& trocas) {
        int pivo = v[dir];
        int i = esq;                        // ponteiro de escrita
        for (int j = esq; j < dir; j++) {   // ponteiro de leitura
            if (v[j] <= pivo) {
                trocar(v, i, j, trocas);
                i++;
            }
        }
        trocar(v, i, dir, trocas);          // pivo vai para a posicao final
        return i;
    }

public:

    tuple<int, vector<int>, ll> findKthLargest(vector<int> nums, int k) {
        int n = nums.size();
        int alvo = n - k;
        ll trocas = 0;

        int esq = 0;
        int dir = n - 1;


        while (esq <= dir) {
            int p = particionarLomuto(nums, esq, dir, trocas);

            if (p == alvo) {
                return {nums[p], nums, trocas};
            } else if (p < alvo) {
                esq = p + 1;
            } else {
                dir = p - 1;
            }
        }
        return {nums[alvo], nums, trocas};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> nums(n);
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
    }

    Solution solucao;
    auto [valor, vetorFinal, trocas] = solucao.findKthLargest(nums, k);

    cout << valor << '\n';
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << ' ';
        cout << vetorFinal[i];
    }
    cout << '\n' << trocas << '\n';

    return 0;
}