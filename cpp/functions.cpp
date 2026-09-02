#include "../headers/functions.h"

vector<BajtVerovatnoca> listaSimbolaVerovatnoca(const vector<double>& pi) {
    vector<BajtVerovatnoca> bajtoviVerovatnoce;
    for (int i = 0; i < 256; ++i) {
        if (pi[i] > 0) {
            bajtoviVerovatnoce.push_back({static_cast<uint8_t>(i), pi[i]});
        }
    }
    return bajtoviVerovatnoce;
}

void insertionSortOpadajuce(vector<BajtVerovatnoca>& bajtoviVerovatnoce) {
    int n = bajtoviVerovatnoce.size();
    for (int i = 1; i < n; ++i) {
        BajtVerovatnoca tempBajt = bajtoviVerovatnoce[i];
        int j = i - 1;
        while (j >= 0 && bajtoviVerovatnoce[j].second < tempBajt.second) {
            bajtoviVerovatnoce[j + 1] = bajtoviVerovatnoce[j];
            j--;
        }
        bajtoviVerovatnoce[j + 1] = tempBajt;
    }
}