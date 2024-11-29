#ifndef SM2024_PROJEKT_KOMPRESJA_H
#define SM2024_PROJEKT_KOMPRESJA_H

#include "Modele.h"
#include <vector>


using namespace std;


struct slowo{
    Uint16 kod =0;
    Uint8 dlugosc = 0;
    Uint8 element[4096];
    bool wSlowniku = false;
};

slowo noweSlowo();
slowo noweSlowo(Uint8 znak);

slowo polaczSlowo(slowo aktualneSlowo, Uint8 znak);

bool porownajSlowa(slowo slowo1, slowo slowo2);

int znajdzWSlowniku(slowo szukany);

void wyswietlSlowo(slowo aktualneSlowo);

int dodajDoSLownika(slowo nowy, bool czyWyswietlac = true);

void LZWinicjalizacja();
void LZWKompresja(vector<Uint8> wejscie, int dlugosc, string fileName);
void LZWDekompresja(string fileName);

void ByteRunKompresja(vector<Uint8> wejscie, int dlugosc, string fileName);
void ByteRunDekompresja(string fileName);

void RLEKompresja(vector<Uint8> wejscie, int dlugosc, string fileName);
void RLEDekompresja(string fileName);

#endif //SM2024_PROJEKT_KOMPRESJA_H
