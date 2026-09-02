#ifndef SHA_FAN
#define SHA_FAN

#include "entropy.h"
#include "functions.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <bitset>

using namespace std;

unordered_map<uint8_t, string> shannonFano(vector<BajtVerovatnoca> bajtoviVerovatnoce);
pair<unordered_map<uint8_t, string>, int> code_sha_fan(const string& input, const string& output);
void decodeSH(const string& coded, const string& decoded,
const unordered_map<uint8_t, string>& codes, int valid);
#endif