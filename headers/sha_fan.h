#ifndef SHA_FAN
#define SHA_FAN

#include "entropy.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <bitset>

using namespace std;
typedef pair<uint8_t, double> BajtVerovatnoca;
unordered_map<uint8_t, string> shannonFano(vector<BajtVerovatnoca> bajtoviVerovatnoce);
pair<unordered_map<uint8_t, string>, int> code_sha_fan(const string& input, const string& output);

vector<BajtVerovatnoca> listaSimbolaVerovatnoca(const vector<double>& pi);
void insertionSortOpadajuce(vector<BajtVerovatnoca>& bajtoviVerovatnoce);
void dekodiranjeFajlSH(const string& kodiraniFajl, const string& dekodiraniFajl,
     const unordered_map<uint8_t, string>& kodovi, int validniBitovi);
#endif