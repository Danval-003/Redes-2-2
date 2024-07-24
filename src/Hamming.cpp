#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include "../include/Hamming.h"
using namespace std;

std::string translateBits(const std::vector<int>& bits) {
    std::string message;

    if (bits.size() % 8 != 0) {
        std::cerr << redShell << "Error: La longitud de la lista de bits debe ser múltiplo de 8." << resetShell << std::endl;
        return "";
    }

    for (size_t i = 0; i < bits.size(); i += 8) {
        std::bitset<8> bitset;
        for (size_t j = 0; j < 8; ++j) {
            bitset[7 - j] = bits[i + j];
        }
        message += static_cast<char>(bitset.to_ulong());
    }

    return message;
}

vector<int> evalBinaryMessage(const string& binaryInitStr, int blockSize) {
    vector<int> binaryBlocks;
    int binaryInitSize = binaryInitStr.size();
    if (binaryInitSize % blockSize != 0) {
        cout << redShell << "The binary message is not a multiple of " << blockSize << resetShell << endl;
        return binaryBlocks;
    }

    for (char ch : binaryInitStr) {
        binaryBlocks.push_back(ch - '0');
    }

    return binaryBlocks;
}

vector<int> ParityBits(int index, int r) {
    // Verify if the number is a power of 2
    if ((index & (index - 1)) == 0) {
        return vector<int>();
    }

    vector<int> binaryIndex(r, 0);
    for (int i = 0; i < r; ++i) {
        binaryIndex[i] = (index >> i) & 1;
    }
    reverse(binaryIndex.begin(), binaryIndex.end());
    return binaryIndex;
}

int ValueOfBinary(const vector<int>& bin) {
    int value = 0;
    for (size_t i = 0; i < bin.size(); ++i) {
        value = (value << 1) | bin[i];
    }
    return value;
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
            parityBits[binaryIndex.size() - j -1] += binaryIndex[j] * binaryBlocks[i];
            parityBits[binaryIndex.size() - j -1] %= 2;
        }
    }

    vector<int> parityOriginalBits(r, 0); 
    for (int i = 0; i < r; i++) {
        int parityBitPosition = (1 << i) - 1;
        parityOriginalBits[i] = binaryBlocks[parityBitPosition];
    }

    int errorPos = 0;
    for (size_t i = 0; i < parityBits.size(); ++i) {
        if (parityBits[i] != parityOriginalBits[parityOriginalBits.size() - i - 1]) {
            errorPos += 1 << i;
        }
    }
    

    if (errorPos > 0) {
        cout << redShell << "Error detected at position: " << errorPos << resetShell << endl;
        binaryBlocks[errorPos - 1] ^= 1;
    }

    vector<int> dataBits;
    for (size_t i = 0; i < binaryBlocks.size(); ++i) {
        if ((i + 1) & (i + 1 - 1)) { // Not a power of 2
            dataBits.push_back(binaryBlocks[i]);
        }
    }

    cout << greenShell << "Not has been detected any error" << resetShell << endl;
    cout << endl;

    string decodedMessage = translateBits(dataBits);

    return decodedMessage;
}
