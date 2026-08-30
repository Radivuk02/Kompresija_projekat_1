#include "../headers/entropy.h"
#include <fstream>
#include <cmath>
#include <vector>
#include <iostream>

pair<double, vector<double>> bajtEntropija(const string& file) {
    ifstream fajl(file, ios::binary);
    if (!fajl.is_open()) {
        cerr << "Greska pri otvaranju fajla: " << file << endl;
        return {0.0, vector<double>(256, 0.0)};
    }

    vector<uint8_t> podaci((istreambuf_iterator<char>(fajl)), istreambuf_iterator<char>());
    fajl.close();

    size_t N = podaci.size();
    if (N == 0) return {0.0, vector<double>(256, 0.0)};

    vector<long long> Ni(256, 0);
    for (uint8_t bajt : podaci) {
        Ni[bajt]++;
    }

    double entropija = 0.0;
    vector<double> pi(256, 0.0);

    for (int i = 0; i < 256; ++i) {
        if (Ni[i] == 0) continue;

        pi[i] = static_cast<double>(Ni[i]) / N;
        entropija -= (pi[i] * log2(pi[i]));
    }

    return {entropija, pi};
}