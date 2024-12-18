// funkcje do operacji na plikach
#ifndef PLIKI_H_INCLUDED
#define PLIKI_H_INCLUDED

#include <stdio.h>
#include <math.h>
#include <fstream>
#include <SDL2/SDL.h>

Uint8 shiftByte(Uint8 b, Uint8 shiftAmount);
void ZapiszPlik(string a);
void OdczytajPlik(string a);

void ZapiszModel(string a, int tryb);
void OdczytajModel(string a);

void ZapiszByteRun(string a, int tryb);
void OdczytajByteRun(string a, int tryb);

template <typename T>
void saveVector(vector<T> v, string fileName){
    ofstream output((fileName + ".z21").c_str(), ios::binary);

    if (!output) {
        cerr << "Nie udało się otworzyć pliku do zapisu: " << fileName << ".z21\n";
        exit(1);
    }


    output.write(reinterpret_cast<char*>(v.data()), v.size()*sizeof(T));
    cout << "Zapisano plik \n";
    output.close();
};

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
