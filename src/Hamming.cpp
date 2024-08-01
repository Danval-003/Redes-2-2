#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include "../include/Hamming.h"
using namespace std;

std::string translateBits(const std::vector<int>& bits) {
    std::string message = "";
    for (size_t i = 0; i < bits.size(); i++) {
        message += (bits[i] == 0) ? "0" : "1";
    }
    return message;
}

vector<int> evalBinaryMessage(const string& binaryInitStr, int blockSize) {
    vector<int> binaryBlocks;
    int binaryInitSize = binaryInitStr.size();
    if (binaryInitSize % blockSize != 0) {
        //cout << "The binary message is not a multiple of " << blockSize << endl;
        return binaryBlocks;
    }

    for (char ch : binaryInitStr) {
        binaryBlocks.push_back(ch - '0');
    }

    return binaryBlocks;
}

vector<int> ParityBits(int index, int r) {
    vector<int> binaryIndex(r, 0);
    for (int i = 0; i < r; ++i) {
        binaryIndex[i] = (index >> i) & 1;
    }
    reverse(binaryIndex.begin(), binaryIndex.end());
    return binaryIndex;
}

string DecoHammingMtoNToBinary(const string& binaryInitStr) {
    int r = 0;
    int m = 0;
    int n = binaryInitStr.size();
    
    for (int i = 0; i < n; i++) {
        int temp = i + 1;
        if ((temp & (temp - 1)) == 0) {
            r++;
        } else {
            m++;
        }
    }

    vector<int> binaryBlocks = evalBinaryMessage(binaryInitStr, n);
    if (binaryBlocks.empty()) {
        return "";
    }

    vector<int> parityBits(r, 0);
    for (size_t i = 0; i < binaryBlocks.size(); ++i) {
        vector<int> binaryIndex = ParityBits(i + 1, r);
        for (size_t j = 0; j < binaryIndex.size(); ++j) {
            parityBits[binaryIndex.size() - j - 1] += binaryIndex[j] * binaryBlocks[i];
            parityBits[binaryIndex.size() - j - 1] %= 2;
        }
    }

    //cout << "Parity bits: ";
    for (size_t i = 0; i < parityBits.size(); ++i) {
        //cout << parityBits[i];
    }
    //cout << endl;

    int errorPos = 0;
    for (size_t i = 0; i < parityBits.size(); ++i) {
        if (parityBits[i] != 0) {
            errorPos += 1 << i;
        }
    }

    if (errorPos > 0) {
        //cout << "Error detected at position: " << errorPos << endl;
        binaryBlocks[errorPos - 1] ^= 1;

        vector<int> parityBits2(r, 0);
        for (size_t i = 0; i < binaryBlocks.size(); ++i) {
            vector<int> binaryIndex = ParityBits(i + 1, r);
            for (size_t j = 0; j < binaryIndex.size(); ++j) {
                parityBits2[binaryIndex.size() - j - 1] += binaryIndex[j] * binaryBlocks[i];
                parityBits2[binaryIndex.size() - j - 1] %= 2;
            }
        }

        //cout << "Parity bits 2: ";
        for (size_t i = 0; i < parityBits2.size(); ++i) {
            //cout << parityBits2[i];
        }
        //cout << endl;

        int errorPos2 = 0;
        for (size_t i = 0; i < parityBits2.size(); ++i) {
            if (parityBits2[i] != 0) {
                errorPos2 += 1 << i;
            }
        }

        if (errorPos2 > 0) {
            //cout << "Multiple errors detected" << endl;
            return "";
        } else {
            //cout << "Error corrected" << endl;
        }
    } else {
        //cout << "No error detected" << endl;
    }

    vector<int> dataBits;
    for (size_t i = 0; i < binaryBlocks.size(); ++i) {
        if ((i & (i + 1)) != 0) {
            dataBits.push_back(binaryBlocks[i]);
        }
    }

    string decodedMessage = translateBits(dataBits);
    return decodedMessage;
}
