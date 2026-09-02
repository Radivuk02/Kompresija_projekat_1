#ifndef LZ77_H
#define LZ77_H

#include "entropy.h"
#include "functions.h"
#include <vector>
#include <string>
#include <cstdint>
#include <tuple>
#include <fstream>
#include <iostream>
using namespace std;

typedef tuple<uint16_t, uint8_t, uint8_t> LZ77Triple;

vector<LZ77Triple> lz77(const vector<uint8_t>& podaci, size_t velicinaProzor = 4096, size_t bajtoviUnapred = 15);
vector<LZ77Triple> codeLZ77(const string& input, const string& output);
void decodeLZ77(const string& kodiraniFajl, const string& dekodiraniFajl);

#endif