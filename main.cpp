#include "headers/entropy.h"
#include "headers/sha_fan.h"
#include "headers/huffman.h"
#include "headers/lz77.h"
#include "headers/lzw.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <map>
#include <vector>
#include <tuple>

namespace fs = std::filesystem;
using namespace std;
using namespace std::chrono;

bool uporediFajlove(const string& f1, const string& f2) {
    ifstream stream1(f1, ios::binary);
    ifstream stream2(f2, ios::binary);

    if (!stream1 || !stream2) return false;

    return equal(istreambuf_iterator<char>(stream1), istreambuf_iterator<char>(),
                 istreambuf_iterator<char>(stream2));
}

double dajVelicinuMB(const string& putanja) {
    if (!fs::exists(putanja)) return 0.0;
    return static_cast<double>(fs::file_size(putanja)) / (1024.0 * 1024.0);
}

template<typename Func>
double izmeriVreme(Func funkcija) {
    auto start = high_resolution_clock::now();
    funkcija();
    auto end = high_resolution_clock::now();
    duration<double, milli> ms = end - start;
    return ms.count();
}

int main() {
    // 1. Kreiranje direktorijuma
    fs::create_directory("compress");
    fs::create_directory("decompress");
    fs::create_directory("codes");

    string input = "input.bin";
    if (!fs::exists(input)) {
        cerr << "Greska: Ulazni fajl '" << input << "' ne postoji u korenskom direktorijumu!" << endl;
        return 1;
    }

    auto [entropija, pi] = bajtEntropija(input);
    double velicinaOriginala = dajVelicinuMB(input);

    cout << "=========================================\n";
    cout << "Ulazni fajl: " << input << " (" << fixed << setprecision(4) << velicinaOriginala << " MB)\n";
    cout << "Entropija fajla: " << setprecision(4) << entropija << " bit/simbol\n";
    cout << "=========================================\n\n";

    ofstream fajlKod;

    // --- 1. SHANNON-FANO ---
    string codedFile = "compress/ShannonFano.bin";
    string decodedFile = "decompress/ShannonFano.bin";
    string codes = "codes/ShannonFano.txt";

    unordered_map<uint8_t, string> kodoviSF;
    int validniBitoviSF = 8;

    double tKompSF = izmeriVreme([&]() {
        tie(kodoviSF, validniBitoviSF) = code_sha_fan(input, codedFile);
    });
    cout << "Shannon-Fano Kodiranje zavrseno.\n";

    fajlKod.open(codes);
    for (const auto& [bajt, kod] : kodoviSF) {
        fajlKod << "Bajt " << static_cast<int>(bajt) << ": " << kod << "\n";
    }
    fajlKod.close();

    double tDekomSF = izmeriVreme([&]() {
        decodeSH(codedFile, decodedFile, kodoviSF, validniBitoviSF);
    });
    cout << "Dekodiranje zavrseno.\n";

    bool statusSF = uporediFajlove(input, decodedFile);
    cout << (statusSF ? "USPESNO" : "NEUSPESNO") << " implementiran Shannon Fano algoritam\n\n";


    // --- 2. HUFFMAN ---
    codedFile = "compress/huffman.bin";
    decodedFile = "decompress/huffman.bin";
    codes = "codes/huffman.txt";

    unordered_map<uint8_t, string> kodoviHuf;
    int validniBitoviHuf = 8;

    double tKompHuf = izmeriVreme([&]() {
        tie(kodoviHuf, validniBitoviHuf) = codeHuffman(input, codedFile);
    });
    cout << "Huffman Kodiranje zavrseno.\n";

    fajlKod.open(codes);
    for (const auto& [bajt, kod] : kodoviHuf) {
        fajlKod << "Bajt " << static_cast<int>(bajt) << ": " << kod << "\n";
    }
    fajlKod.close();

    double tDekomHuf = izmeriVreme([&]() {
        decodeHuffman(codedFile, decodedFile, kodoviHuf, validniBitoviHuf);
    });
    cout << "Dekodiranje zavrseno.\n";

    bool statusHuf = uporediFajlove(input, decodedFile);
    cout << (statusHuf ? "USPESNO" : "NEUSPESNO") << " implementiran Huffmanov algoritam\n\n";


    // --- 3. LZ77 ---
    // --- 3. LZ77 ---
    codedFile = "compress/lz77.bin";
    decodedFile = "decompress/lz77.bin";
    codes = "codes/lz77.txt";

    // 1. Deklarišemo odgovarajući tip koji funkcija codeLZ77 zapravo vraća
    vector<LZ77Triple> kodoviLZ77;

    double tKompLZ77 = izmeriVreme([&]() {
        kodoviLZ77 = codeLZ77(input, codedFile);
    });
    cout << "LZ77 Kodiranje zavrseno.\n";

fajlKod.open(codes);
    // Dekonstrukcija tuple-a na individualne promenljive
    for (const auto& [pomak, duzina, sledeci] : kodoviLZ77) {
        fajlKod << "(" << pomak << ", " 
                << static_cast<int>(duzina) << ", " 
                << static_cast<int>(sledeci) << ")\n";
    }
    fajlKod.close();
    fajlKod.close();

    double tDekomLZ77 = izmeriVreme([&]() {
        decodeLZ77(codedFile, decodedFile);
    });

    bool statusLZ77 = uporediFajlove(input, decodedFile);
    cout << (statusLZ77 ? "USPESNO" : "NEUSPESNO") << " implementiran LZ77 algoritam\n\n";
    // --- 4. LZW ---
    codedFile = "compress/lzw.bin";
    decodedFile = "decompress/lzw.bin";
    codes = "codes/lzw.txt";

    map<vector<uint8_t>, int> kodoviLZW;
    int validniBitoviLZW = 8;

    double tKompLZW = izmeriVreme([&]() {
        tie(kodoviLZW, validniBitoviLZW) = codeLZW(input, codedFile);
    });
    cout << "LZW Kodiranje zavrseno.\n";

    fajlKod.open(codes);
    for (const auto& [niz, kod] : kodoviLZW) {
        fajlKod << "BAJT [";
        for (uint8_t b : niz) fajlKod << static_cast<int>(b) << " ";
        fajlKod << "]: Kod " << kod << "\n";
    }
    fajlKod.close();

    double tDekomLZW = izmeriVreme([&]() {
        decodeLZW(codedFile, decodedFile, kodoviLZW, validniBitoviLZW);
    });
    cout << "Dekodiranje zavrseno.\n";

    bool statusLZW = uporediFajlove(input, decodedFile);
    cout << (statusLZW ? "USPESNO" : "NEUSPESNO") << " implementiran LZW algoritam\n\n";


    // --- 5. GENERISANJE IZVEŠTAJA (izvestaj.txt) ---
    ofstream izvestaj("izvestaj.txt");
    izvestaj << "========================================================================================\n";
    izvestaj << "                            IZVESTAJ O KOMPRESIJI PODATAKA                              \n";
    izvestaj << "========================================================================================\n\n";
    izvestaj << "Ulazni fajl: " << input << "\n";
    izvestaj << "Velicina originala: " << fixed << setprecision(4) << velicinaOriginala << " MB\n";
    izvestaj << "Bajt-Entropija: " << setprecision(4) << entropija << " bit/simbol\n\n";

    izvestaj << left << setw(15) << "Algoritam" 
             << setw(16) << "Komp.(MB)" 
             << setw(14) << "Stepen" 
             << setw(16) << "Vreme K.(ms)" 
             << setw(16) << "Vreme D.(ms)" 
             << setw(10) << "Status" << "\n";
    izvestaj << "----------------------------------------------------------------------------------------\n";

    auto upisiRed = [&](string naziv, string path, double tKomp, double tDekom, bool pass) {
        double sz = dajVelicinuMB(path);
        double stepen = (sz > 0) ? (velicinaOriginala / sz) : 0.0;
        izvestaj << left << setw(15) << naziv 
                 << setw(16) << fixed << setprecision(4) << sz 
                 << setw(14) << setprecision(2) << stepen 
                 << setw(16) << setprecision(2) << tKomp 
                 << setw(16) << setprecision(2) << tDekom 
                 << setw(10) << (pass ? "USPESNO" : "NEUSPESNO") << "\n";
    };

    upisiRed("Shannon-Fano", "compress/ShannonFano.bin", tKompSF, tDekomSF, statusSF);
    upisiRed("Huffman", "compress/huffman.bin", tKompHuf, tDekomHuf, statusHuf);
    upisiRed("LZ77", "compress/lz77.bin", tKompLZ77, tDekomLZ77, statusLZ77);
    upisiRed("LZW", "compress/lzw.bin", tKompLZW, tDekomLZW, statusLZW);

    izvestaj << "\nUPUTSTVO ZA POKRETANJE IMPLEMENTACIJE:\n";
    izvestaj << "1. Postaviti 'input.bin' u korenski direktorijum projekta.\n";
    izvestaj << "2. Otvoriti terminal i pozicionirati se u projekat.\n";
    izvestaj << "3. Kreirati build folder: mkdir -p build && cd build\n";
    izvestaj << "4. Kompajlirati kod: cmake .. && make\n";
    izvestaj << "5. Pokrenuti executable sa: ./main (iz build foldera)\n";
    izvestaj.close();

    cout << "=========================================\n";
    cout << "Proces zavrsen! Izvestaj uspesno generisan u 'izvestaj.txt'.\n";
    cout << "=========================================\n";

    return 0;
}