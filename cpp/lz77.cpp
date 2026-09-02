#include "../headers/lz77.h"

vector<LZ77Triple> lz77(const vector<uint8_t>& podaci, size_t velicinaProzor, size_t bajtoviUnapred) {
    vector<LZ77Triple> kod;
    size_t n = podaci.size();
    if (n == 0) return kod;

    size_t i = 0;
    while (i < n) {
        if (i == n - 1) {
            kod.push_back({0, 0, podaci[i]});
            break;
        }

        size_t maxDuzina = min(bajtoviUnapred, n - i - 1);
        size_t pProzor = (i > velicinaProzor) ? (i - velicinaProzor) : 0;

        uint16_t najPomak = 0;
        uint8_t najDuzina = 0;

        for (size_t p = pProzor; p < i; ++p) {
            size_t l = 0;
            while (l < maxDuzina && podaci[p + l] == podaci[i + l]) {
                l++;
            }
            if (l > najDuzina) {
                najDuzina = static_cast<uint8_t>(l);
                najPomak = static_cast<uint16_t>(i - p);
                if (najDuzina == maxDuzina) break;
            }
        }

        size_t sledeciI = i + najDuzina;
        uint8_t sledeci = (sledeciI < n) ? podaci[sledeciI] : 0;

        kod.push_back({najPomak, najDuzina, sledeci});
        i += najDuzina + 1;
    }

    return kod;
}

vector<LZ77Triple> codeLZ77(const string& input, const string& output) {
    ifstream fajlUlaz(input, ios::binary);
    vector<uint8_t> podaci((istreambuf_iterator<char>(fajlUlaz)), istreambuf_iterator<char>());
    fajlUlaz.close();

    auto kodovi = lz77(podaci);

    ofstream fajlIzlaz(output, ios::binary);
    uint32_t brojKodova = static_cast<uint32_t>(kodovi.size());


    uint8_t buffer4[4] = {
        static_cast<uint8_t>((brojKodova >> 24) & 0xFF),
        static_cast<uint8_t>((brojKodova >> 16) & 0xFF),
        static_cast<uint8_t>((brojKodova >> 8) & 0xFF),
        static_cast<uint8_t>(brojKodova & 0xFF)
    };
    fajlIzlaz.write(reinterpret_cast<char*>(buffer4), 4);

    for (const auto& [pomak, duzina, sledeci] : kodovi) {
        uint8_t buffer2[2] = {
            static_cast<uint8_t>((pomak >> 8) & 0xFF),
            static_cast<uint8_t>(pomak & 0xFF)
        };
        fajlIzlaz.write(reinterpret_cast<char*>(buffer2), 2);
        fajlIzlaz.write(reinterpret_cast<const char*>(&duzina), 1);
        fajlIzlaz.write(reinterpret_cast<const char*>(&sledeci), 1);
    }

    fajlIzlaz.close();
    return kodovi;
}

void decodeLZ77(const string& kodiraniFajl, const string& dekodiraniFajl)
{
    std::ifstream fajlUlaz(kodiraniFajl, std::ios::binary);
    std::vector<uint8_t> podaci((std::istreambuf_iterator<char>(fajlUlaz)), std::istreambuf_iterator<char>());
    fajlUlaz.close();

    std::ofstream fajlIzlaz(dekodiraniFajl, std::ios::binary);
    if (podaci.size() < 4) return;

    size_t pozicija = 0;
    uint32_t brojKodova = (podaci[0] << 24) | (podaci[1] << 16) | (podaci[2] << 8) | podaci[3];
    pozicija += 4;

    std::vector<uint8_t> dekodiraniBajtovi;

    for (uint32_t z = 0; z < brojKodova; ++z) {
        if (pozicija + 4 > podaci.size()) break;

        uint16_t pomak = (podaci[pozicija] << 8) | podaci[pozicija + 1];
        uint8_t duzina = podaci[pozicija + 2];
        uint8_t simbol = podaci[pozicija + 3];
        pozicija += 4;

        if (pomak > 0 && dekodiraniBajtovi.size() >= pomak) {
            for (uint8_t _i = 0; _i < duzina; ++_i) {
                uint8_t bajt = dekodiraniBajtovi[dekodiraniBajtovi.size() - pomak];
                dekodiraniBajtovi.push_back(bajt);
            }
        }
        dekodiraniBajtovi.push_back(simbol);
    }

    fajlIzlaz.write(reinterpret_cast<char*>(dekodiraniBajtovi.data()), dekodiraniBajtovi.size());
    fajlIzlaz.close();
}