#ifndef LZW_H
#define LZW_H

#include "entropy.h"
#include "functions.h"
#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <bitset>
#include <unordered_map>

using namespace std;
pair<map<vector<uint8_t>, int>, int> codeLZW(const string& input, const string& output);
void decodeLZW(const string& kodiraniFajl, const string& dekodiraniFajl, const map<vector<uint8_t>, int>& kodovi, int validniBitovi);

#endif