// funkcje do operacji na plikach
#ifndef PLIKI_H_INCLUDED
#define PLIKI_H_INCLUDED

#include <stdio.h>
#include <math.h>
#include <fstream>
#include <SDL2/SDL.h>

Uint8 shiftByte(Uint8 b, Uint8 shiftAmount);
void ZapiszPlik(string a, Uint8 tryb);
void OdczytajPlik(string a);

/**
 * Saves vector to the file
 * @tparam T vector type
 * @param v1 vector
 * @param fileName name of the file (without extension)
 */
template <typename T>
void saveVector(vector<T> v1, string fileName){
    ofstream output((fileName + ".z21").c_str(), ios::binary);

    if (!output) {
        cerr << "Nie udało się otworzyć pliku do zapisu: " << fileName << ".z21\n";
        exit(1);
    }


    output.write(reinterpret_cast<char*>(v1.data()), v1.size()*sizeof(T));

    cout << "Zapisano plik \n";
    output.close();
};

/**
 * Reads values from file to a vector
 * @tparam T vector type
 * @param fileName name of the file (without extension)
 * @return vector of values
 */
template <typename T>
vector<T> readVector(string fileName){
    ifstream input((fileName + ".z21").c_str(), ios::binary | ios::ate);
    //wejscie.read( (char*)&paleta5[i].r, sizeof(Uint8));
    if (!input) {
        cerr << "Nie udało się otworzyć pliku: " << fileName << ".z21\n";
        exit(1);
    }

    streamsize fileSize = input.tellg();

    input.seekg(0, ios::beg);

    vector<T> buff(fileSize / sizeof(T));

    if(!input.read(reinterpret_cast<char*>(buff.data()), fileSize)){
        cerr << "Bład podczas odczytu\n";
        exit(1);
    }

    cout << "Odczytano plik\n";

    return buff;
};


#endif // SM2024_PLIKI_H_INCLUDED
