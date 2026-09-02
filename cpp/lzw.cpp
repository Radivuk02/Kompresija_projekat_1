#include "../headers/lzw.h"


pair<map<vector<uint8_t>, int>, int> codeLZW(const string& input, const string& output) {
    ifstream fajlUlaz(input, ios::binary);
    vector<uint8_t> podaci((istreambuf_iterator<char>(fajlUlaz)), istreambuf_iterator<char>());
    fajlUlaz.close();

    map<vector<uint8_t>, int> recnik;
    for (int i = 0; i < 256; ++i) {
        recnik[{static_cast<uint8_t>(i)}] = i;
    }
    int velicinaRecnika = 256;
    int velicinaRecnikaMax = 4096;

    vector<int> kodiraniPodaci;
    vector<uint8_t> bajtoviNiz;

    for (uint8_t bajt : podaci) {
        vector<uint8_t> temp = bajtoviNiz;
        temp.push_back(bajt);

        if (recnik.find(temp) != recnik.end()) {
            bajtoviNiz = temp;
        } else {
            if (!bajtoviNiz.empty()) {
                kodiraniPodaci.push_back(recnik[bajtoviNiz]);
            }
            if (velicinaRecnika < velicinaRecnikaMax) {
                recnik[temp] = velicinaRecnika++;
            }
            bajtoviNiz = {bajt};
        }
    }
    if (!bajtoviNiz.empty()) {
        kodiraniPodaci.push_back(recnik[bajtoviNiz]);
    }

    ofstream fajlIzlaz(output, ios::binary);
    string kodiraniPodaciBafer = "";

    for (int kod : kodiraniPodaci) {
        kodiraniPodaciBafer += bitset<12>(kod).to_string();
        while (kodiraniPodaciBafer.length() >= 8) {
            uint8_t bajt = static_cast<uint8_t>(stoi(kodiraniPodaciBafer.substr(0, 8), nullptr, 2));
            fajlIzlaz.write(reinterpret_cast<char*>(&bajt), 1);
            kodiraniPodaciBafer = kodiraniPodaciBafer.substr(8);
        }
    }

    int validniBitovi = 8;
    if (!kodiraniPodaciBafer.empty()) {
        validniBitovi = kodiraniPodaciBafer.length();
        kodiraniPodaciBafer.append(8 - validniBitovi, '0');
        uint8_t bajt = static_cast<uint8_t>(stoi(kodiraniPodaciBafer, nullptr, 2));
        fajlIzlaz.write(reinterpret_cast<char*>(&bajt), 1);
    }

    fajlIzlaz.close();
    return {recnik, validniBitovi};
}

void decodeLZW(const string& kodiraniFajl, const string& dekodiraniFajl, const map<vector<uint8_t>, int>& kodovi, int validniBitovi) {
    unordered_map<int, vector<uint8_t>> recnik;
    for (const auto& [bajtovi, kod] : kodovi) {
        recnik[kod] = bajtovi;
    }

    ifstream fajl(kodiraniFajl, ios::binary);
    vector<uint8_t> bajtovi((istreambuf_iterator<char>(fajl)), istreambuf_iterator<char>());
    fajl.close();

    string bitovi = "";
    for (size_t i = 0; i < bajtovi.size(); ++i) {
        string bBits = bitset<8>(bajtovi[i]).to_string();
        if (i == bajtovi.size() - 1) {
            bitovi += bBits.substr(0, validniBitovi);
        } else {
            bitovi += bBits;
        }
    }

    vector<uint8_t> dekodiraniBajtovi;
    while (bitovi.length() >= 12) {
        string uzorak = bitovi.substr(0, 12);
        int kod = stoi(uzorak, nullptr, 2);
        if (recnik.find(kod) != recnik.end()) {
            const auto& val = recnik[kod];
            dekodiraniBajtovi.insert(dekodiraniBajtovi.end(), val.begin(), val.end());
        }
        bitovi = bitovi.substr(12);
    }

    ofstream fajlIzlaz(dekodiraniFajl, ios::binary);
    fajlIzlaz.write(reinterpret_cast<char*>(dekodiraniBajtovi.data()), dekodiraniBajtovi.size());
    fajlIzlaz.close();
}