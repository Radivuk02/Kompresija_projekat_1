#include "headers/entropy.h"
#include "headers/sha_fan.h"
#include "headers/huffman.h"
#include "headers/lz77.h"
#include<iostream>
#include<fstream>
#include <filesystem>

using namespace std;

bool uporediFajlove(const string& f1, const string& f2) {
    ifstream stream1(f1, ios::binary);
    ifstream stream2(f2, ios::binary);

    if (!stream1 || !stream2) return false;

    return equal(istreambuf_iterator<char>(stream1), istreambuf_iterator<char>(),
                      istreambuf_iterator<char>(stream2));
}


int main(){
    string input="input.bin";
    auto[entropija,pi]=bajtEntropija(input);
    cout<<"Entropija fajla je:"<<entropija<<" bit/simbol"<<endl;
    
    string codedFile = "compress/ShannonFano.bin";
    string decodedFile = "decompress/ShannonFano.bin";
    string codes = "codes/ShannonFano.txt";

    auto [kodovi, validniBitovi] = code_sha_fan(input, codedFile);
    cout << "Shannon-Fano Kodiranje zavrseno.\n";

    ofstream fajlKod(codes);
    for (const auto& [bajt, kod] : kodovi) {
        fajlKod << "Bajt " << static_cast<int>(bajt) << ": " << kod << "\n";
    }
    fajlKod.close();
    decodeSH(codedFile, decodedFile, kodovi, validniBitovi);
    cout << "Dekodiranje zavrseno.\n";

    if (uporediFajlove(input,decodedFile)) {
        cout << "USPESNO implementiran Shannon Fano algoritam\n";
    } else {
        cout << "NEUSPESNO implementiran Shannon Fano algoritam\n";
    }


    codedFile = "compress/huffman.bin";
    decodedFile = "decompress/huffman.bin";
    codes = "codes/huffman.txt";

    tie(kodovi, validniBitovi) = codeHuffman(input, codedFile);
    cout << "Huffman Kodiranje zavrseno.\n";
    fajlKod.open(codes);
    for (const auto& [bajt, kod] : kodovi) {
        fajlKod << "Bajt " << static_cast<int>(bajt) << ": " << kod << "\n";
    }

    decodeSH(codedFile, decodedFile, kodovi, validniBitovi);
    cout << "Dekodiranje zavrseno.\n";

    if (uporediFajlove(input, decodedFile)) {
        cout << "USPESNO implementiran Huffmanov algoritam\n";
    } else {
        cout << "NEUSPESNO implementiran Huffmanov algoritam\n";
    }

    codedFile = "compress/lz77.bin";
    decodedFile = "decompress/lz77.bin";
    codes = "codes/lz77.txt";

    auto kodoviLZ77 = codeLZ77(input, codedFile);
    cout << "LZ77 Kodiranje zavrseno.\n";

    fajlKod.open(codes);
    for (const auto& [pomak, duzina, sledeci] : kodoviLZ77) {
        fajlKod << "(" << pomak << ", " << static_cast<int>(duzina) << ", " << static_cast<int>(sledeci) << ")\n";
    }
    fajlKod.close();

    decodeLZ77(codedFile, decodedFile);

    if (uporediFajlove(input, decodedFile)) {
        cout << "USPESNO implementiran LZ77 algoritam\n";
    } else {
        cout << "NEUSPESNO implementiran LZ77 algoritam\n";
    }

    return 0;

}