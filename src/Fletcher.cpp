#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <bitset>

using namespace std;
const string redShell2 = "\033[1;31m";
const string greenShell2 = "\033[1;32m";
const string blueShell2 = "\033[1;34m";
const string resetShell2 = "\033[0m";

vector<int*> evalBinaryMessage8(const string& binaryInitStr) {
    vector<int*> binaryBlocks;
    int binaryInitSize = binaryInitStr.size();

    // Verifica si el tamaño del mensaje binario es múltiplo de 8
    if (binaryInitSize % 8 != 0) {
        cout << "The binary message is not a multiple of 8" << endl;
        return binaryBlocks;
    }

    for (int i = 0; i < binaryInitSize; i += 8) {
        int* binaryBlock = new int[8];
        for (int j = 0; j < 8; j++) {
            binaryBlock[j] = binaryInitStr[i + j] - '0';
        }
        binaryBlocks.push_back(binaryBlock);
    }

    return binaryBlocks;
}

int ValueOfBlock(int* binary) {
    int value = 0;
    for (int i = 0; i < 8; i++) {
        value += binary[i] << (7 - i);
    }
    return value;
}

string NumberToBinary(int number) {
    string binary = "";
    for (int i = 0; i < 8; i++) {
        binary = to_string(number % 2) + binary;
        number /= 2;
    }
    return binary;
}

std::string translateBits8(const std::vector<int>& bits) {
    std::string message;

    // Asegúrate de que la longitud de bits es múltiplo de 8
    if (bits.size() % 8 != 0) {
        std::cerr << redShell2 << "Error: La longitud de la lista de bits debe ser múltiplo de 8." << resetShell2 << std::endl;
        return "";
    }

    // Iterar sobre la lista de bits en grupos de 8 bits
    for (size_t i = 0; i < bits.size(); i += 8) {
        std::bitset<8> bitset;

        // Llenar el bitset con los 8 bits correspondientes
        for (size_t j = 0; j < 8; ++j) {
            bitset[7 - j] = bits[i + j];  // Establecer el bit en la posición correcta
        }

        // Convertir el número binario a un carácter y agregarlo al mensaje
        message += static_cast<char>(bitset.to_ulong());
    }

    std::cout << "letter: " << blueShell2 << message << resetShell2 << std::endl;
    
    return message;
}

string DecoFletchertoString(const string& binary) {
    vector<int*> binaryBlocks = evalBinaryMessage8(binary);
    if (binaryBlocks.empty()) {
        return "";
    }
     
    if (binaryBlocks.size() == 1) {
        return "";
    }

    // Obtain first block
    int *firstBlock = binaryBlocks[0];
    // Convert block to number
    int firstBlockValue = ValueOfBlock(firstBlock);

    // Delete first block
    binaryBlocks.erase(binaryBlocks.begin());

    int sum1 = 0;
    int sum2 = 0;

    string message = "";

    for (auto binary : binaryBlocks) {
        sum1 += ValueOfBlock(binary);
        sum1 %= 255;
        sum2 += sum1;
        sum2 %= 255;
        message += translateBits8(vector<int>(binary, binary + 8));
    }

    cout << "The sum1 is: " << sum1 << endl;
    cout << "The sum2 is: " << sum2 << endl;

    int checksum = (sum2 << 4) | (sum1 & 0x0F);
    checksum = checksum & 0xFF;
    cout << "The checksum is: " << checksum << endl;

    // Verify if the checksum is equal of the first block
    if (firstBlockValue != checksum) {
        cout << redShell2 << "The checksum is not equal to the first block" << resetShell2 << endl;
        return "";
    }

    cout << greenShell2 << "The checksum is equal to the first block" << resetShell2 << endl;

    // 
    return message;
}