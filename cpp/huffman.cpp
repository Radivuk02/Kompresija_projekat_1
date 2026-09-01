#include "../headers/huffman.h"


void huffCodes(shared_ptr<Node> root, const string& prefiks, unordered_map<uint8_t, string>& kodovi) {
    if (!root) return;

    if (root->isLeaf) {
        kodovi[root->bajt] = prefiks.empty() ? "0" : prefiks;
    } else {
        huffCodes(root->levo, prefiks + "0", kodovi);
        huffCodes(root->desno, prefiks + "1", kodovi);
    }
}

pair<unordered_map<uint8_t, string>, int> codeHuffman(const string& ulazniFajl, const string& izlazniFajl) {
    auto [entropija, pi] = bajtEntropija(ulazniFajl);
    auto bajtoviVerovatnoce = listaSimbolaVerovatnoca(pi);

    vector<shared_ptr<Node>> cvorovi;
    for (const auto& bv : bajtoviVerovatnoce) {
        cvorovi.push_back(make_shared<Node>(bv.first, bv.second));
    }

    if (cvorovi.empty()) return {{}, 8};

    while (cvorovi.size() > 1) {
        sort(cvorovi.begin(), cvorovi.end(), [](const shared_ptr<Node>& a, const shared_ptr<Node>& b) {
            return a->verovatnoca > b->verovatnoca;
        });

        auto c1 = cvorovi.back(); cvorovi.pop_back();
        auto c2 = cvorovi.back(); cvorovi.pop_back();

        auto noviCvor = make_shared<Node>(c1, c2);
        cvorovi.push_back(noviCvor);
    }

    unordered_map<uint8_t, string> kodovi;
    huffCodes(cvorovi[0], "", kodovi);

    ifstream fajlUlaz(ulazniFajl, ios::binary);
    ofstream fajlIzlaz(izlazniFajl, ios::binary);

    vector<uint8_t> podaci((istreambuf_iterator<char>(fajlUlaz)), istreambuf_iterator<char>());
    fajlUlaz.close();

    string kodiraniPodaciBafer = "";
    for (uint8_t bajt : podaci) {
        kodiraniPodaciBafer += kodovi[bajt];

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
    return {kodovi, validniBitovi};
}

