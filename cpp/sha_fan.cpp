
#include "../headers/sha_fan.h"



unordered_map<uint8_t, string> shannonFano(vector<BajtVerovatnoca> bajtoviVerovatnoce){
    if (bajtoviVerovatnoce.size() == 1) {
        return {{bajtoviVerovatnoce[0].first, ""}};
    }

    insertionSortOpadajuce(bajtoviVerovatnoce);

    double ukupnaVerovatnocaGrupe = 0;
    for (const auto& item : bajtoviVerovatnoce) {
        ukupnaVerovatnocaGrupe += item.second;
    }

    double tempSuma = 0;
    size_t i = 0;
    for (i = 0; i < bajtoviVerovatnoce.size(); ++i) {
        tempSuma += bajtoviVerovatnoce[i].second;
        if (tempSuma >= ukupnaVerovatnocaGrupe / 2.0) {
            break;
        }
    }

    vector<BajtVerovatnoca> levaGrupa(bajtoviVerovatnoce.begin(), bajtoviVerovatnoce.begin() + i + 1);
    vector<BajtVerovatnoca> desnaGrupa(bajtoviVerovatnoce.begin() + i + 1, bajtoviVerovatnoce.end());

    auto leviKod = shannonFano(levaGrupa);
    auto desniKod = shannonFano(desnaGrupa);

    unordered_map<uint8_t, string> kodovi;
    for (const auto& pair : leviKod) {
        kodovi[pair.first] = "0" + pair.second;
    }
    for (const auto& pair : desniKod) {
        kodovi[pair.first] = "1" + pair.second;
    }

    return kodovi;
}

pair<unordered_map<uint8_t, string>, int> code_sha_fan(const string& input, const string& output) {
    auto [entropija, pi] = bajtEntropija(input);
    auto bajtoviVerovatnoce = listaSimbolaVerovatnoca(pi);
    auto kod = shannonFano(bajtoviVerovatnoce);

    ifstream fajlUlaz(input, ios::binary);
    ofstream fajlIzlaz(output, ios::binary);

    vector<uint8_t> podaci((istreambuf_iterator<char>(fajlUlaz)), istreambuf_iterator<char>());
    fajlUlaz.close();

    string kodiraniPodaciBafer = "";
    for (uint8_t bajt : podaci) {
        kodiraniPodaciBafer += kod[bajt];

        while (kodiraniPodaciBafer.length() >= 8) {
            uint8_t bajtZaUpis = static_cast<uint8_t>(stoi(kodiraniPodaciBafer.substr(0, 8), nullptr, 2));
            fajlIzlaz.write(reinterpret_cast<char*>(&bajtZaUpis), 1);
            kodiraniPodaciBafer = kodiraniPodaciBafer.substr(8);
        }
    }

    int validniBitovi = 8;
    if (!kodiraniPodaciBafer.empty()) {
        validniBitovi = kodiraniPodaciBafer.length();
        kodiraniPodaciBafer.append(8 - validniBitovi, '0');
        uint8_t bajtZaUpis = static_cast<uint8_t>(stoi(kodiraniPodaciBafer, nullptr, 2));
        fajlIzlaz.write(reinterpret_cast<char*>(&bajtZaUpis), 1);
    }

    fajlIzlaz.close();
    return {kod, validniBitovi};
}

void dekodiranjeFajlSH(const string& kodiraniFajl, const string& dekodiraniFajl,
     const unordered_map<uint8_t, string>& kodovi, int validniBitovi) {
    unordered_map<string, uint8_t> recnik;
    for (const auto& [bajt, kod] : kodovi) {
        recnik[kod] = bajt;
    }

    ifstream fajl(kodiraniFajl, ios::binary);
    vector<uint8_t> bajtovi((istreambuf_iterator<char>(fajl) ), istreambuf_iterator<char>());
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

    string uzorak = "";
    vector<uint8_t> dekodiraniBajtovi;

    for (char bit : bitovi) {
        uzorak += bit;
        if (recnik.find(uzorak) != recnik.end()) {
            dekodiraniBajtovi.push_back(recnik[uzorak]);
            uzorak = "";
        }
    }

    ofstream fajlIzlaz(dekodiraniFajl, ios::binary);
    fajlIzlaz.write(reinterpret_cast<char*>(dekodiraniBajtovi.data()), dekodiraniBajtovi.size());
    fajlIzlaz.close();
}

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