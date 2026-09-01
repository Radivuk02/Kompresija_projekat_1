#include "headers/entropy.h"
#include "headers/sha_fan.h"
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

    decodeSH(codedFile, decodedFile, kodovi, validniBitovi);
    cout << "Dekodiranje zavrseno.\n";

    if (uporediFajlove(input,decodedFile)) {
        cout << "USPESNO implementiran Shannon Fano algoritam\n";
    } else {
        cout << "NEUSPESNO implementiran Shannon Fano algoritam\n";
    }
    return 0;

}