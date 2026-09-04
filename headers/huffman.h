#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "entropy.h"
#include "sha_fan.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <cstdint>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

    struct Node {
        bool isLeaf;
        uint8_t bajt;
        double verovatnoca;
        shared_ptr<Node> levo;
        shared_ptr<Node> desno;

        Node(uint8_t b, double v) : isLeaf(true), bajt(b), verovatnoca(v), levo(nullptr), desno(nullptr) {}
        Node(shared_ptr<Node> l, shared_ptr<Node> d) 
            : isLeaf(false), bajt(0), verovatnoca(l->verovatnoca + d->verovatnoca), levo(l), desno(d) {}
    };

    void huffCodes(shared_ptr<Node> root, const string& prefiks, unordered_map<uint8_t, string>& kodovi);
    pair<unordered_map<uint8_t, string>, int> codeHuffman(const string& input, const string& output);

    void decodeHuffman(const string& kodiraniFajl, const string& dekodiraniFajl, const unordered_map<uint8_t, string>& kodovi, int validniBitovi);
#endif