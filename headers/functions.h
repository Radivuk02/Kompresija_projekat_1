#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include <utility>
#include <cstdint>
using namespace std;
typedef pair<uint8_t, double> BajtVerovatnoca;

vector<BajtVerovatnoca> listaSimbolaVerovatnoca(const vector<double>& pi);
void insertionSortOpadajuce(vector<BajtVerovatnoca>& bajtoviVerovatnoce);
#endif