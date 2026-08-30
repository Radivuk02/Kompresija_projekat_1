#include "headers/entropy.h"
#include<iostream>
#include<fstream>
#include <filesystem>

using namespace std;

int main(){
    string file="input.bin";
    auto[entropija,pi]=bajtEntropija(file);
    cout<<"Entropija fajla je:"<<entropija<<" bit/simbol"<<endl;

    return 0;

}