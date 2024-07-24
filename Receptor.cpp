#include <iostream>
#include <fstream>
#include <string>
#include "Include/Colors.h"
#include "Include/Hamming.h"
#include "Include/Fletcher.h"
using namespace std;


int main(){
    cout << "---------------------------------------------------------------------" << endl;
    cout << blueShell << "Welcome to the receptor" << resetShell << endl;
    

    while (true)
    {
        cout << "---------------------------------------------------------------------" << endl;
        cout << "What do you want to do?" << endl;
        cout << "1. Use Hamming" << endl;
        cout << "2. Use Fletcher" << endl;
        cout << "3. Exit" << endl;

        int option;
        cin >> option;
        if (option == 3)
        {
            break;
        }

        string binary;

        switch (option)
        
        {
        case 1:
            {
                cout << "Enter the binary message: ";
                cin >> binary;
                string message = DecoHammingMtoNToBinary(binary);
                cout << "The message is: " << blueShell << message << resetShell << endl;
                cout << endl;
            }

            break;

        case 2:
            {
                cout << "Enter the binary message: ";
                cin >> binary;
                string message = DecoFletchertoString(binary);
                cout << "The message is: " << greenShell << message << resetShell << endl;
                cout << endl;
            }
        
        default:
            break;
        }

    }
    

    
    return 0;
}
