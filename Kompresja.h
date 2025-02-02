#ifndef SM2024_PROJEKT_KOMPRESJA_H
#define SM2024_PROJEKT_KOMPRESJA_H

#include "Modele.h"
#include <vector>


using namespace std;

struct token8 {
    Uint16 tokLength;
    Uint16 shift;
    Uint8 rawValue;
    token8() : tokLength(0), shift(0), rawValue(0) {}
    token8(Uint16 tokLength, Uint16 shift, Uint8 rawValue): shift(shift), tokLength(tokLength), rawValue(rawValue) {};
};

struct token16 {
    Uint16 tokLength;
    Uint16 shift;
    Uint16 rawValue;
    token16() : tokLength(0), shift(0), rawValue(0) {}
    token16(Uint16 tokLength, Uint16 shift, Uint16 rawValue): shift(shift), tokLength(tokLength), rawValue(rawValue) {};
};

vector<token8> LZ77Kompresja(vector<Uint8> input, int length);

// Funkcja LZ77 - dekompresuje wejściowy wektor danych
vector<Uint8> LZ77Dekompresja(vector<token8> tokens);

vector<token16> LZ77Kompresja(vector<Uint16> input, int length);

// Funkcja LZ77 - dekompresuje wejściowy wektor danych
vector<Uint16> LZ77Dekompresja(vector<token16> tokens);

/** Stuct representing single word, used in LZW compression & decompression
 */
struct slowo{
    Uint16 kod =0;
    Uint8 length = 0;
    Uint8 element[4096];
    bool wSlowniku = false;
};

/** Default constructor for slowo struct,
 * creating an empty slowo
 */
slowo noweSlowo();

/** Constructor for slowo struct,
 * creating a slowo with one element
 * @param   znak        starting element for a slowo
 */
slowo noweSlowo(Uint8 znak);

/** For adding elements to slowo struct
 * @param   aktualneSlowo       slowo struct
 * @param   znak                element to add
 * @return  slowo               slowo struct with a new element
 */
slowo polaczSlowo(slowo aktualneSlowo, Uint8 znak);

/** For comparing two words (slowo structs)
 * @param   slowo1      1st word
 * @param   slowo2      2nd word
 * @return  true        if words are identical;
 *          false       if words are not identical
 */
bool porownajSlowa(slowo slowo1, slowo slowo2);

/** Finds words in dictionary
 * @param   szukany     word to look for
 * @return  index of a word     if the word exists
 *          -1                  if word not found
 */
int znajdzWSlowniku(slowo szukany);

/** Prints word to a console
 * @param   aktualneSlowo   word to print
 */
void wyswietlSlowo(slowo aktualneSlowo);

/** Adds a word to a dictionary
 * @param   slowo           word to add
 * @param   czyWyswietlac   true to print the added word
 *                          false not to print
 *                          (default: false)
 * @return  index of the word in a dictionary
 *          -1 if dictionary overflow
 */
int dodajDoSLownika(slowo nowy, bool czyWyswietlac = false);

/** Init function for LZW compression, sets up the dictionary
 * runs always before compression / decompression
 */
void LZWinicjalizacja();

/** LZW Compression and save to file
 * @param   input       vector of Uint8 BW pixels to compress
 * @param   length      vector size
 * @param   fileName    name of a file to save compressed output to
 */
vector<Uint16> LZWKompresja(vector<Uint8> input, int length);

/** LZW Decompression from a file
 * @param   fileName        name of a file to read compressed input from
 */
vector<Uint8> LZWDekompresja(vector<Uint16> skompresowane);

/** ByteRun Compression and save to a file
 * @param   input       vector of Uint8 BW pixels to compress
 * @param   length      vector size
 * @param   fileName    name of a file to save compressed output to
 */
vector<Sint8> ByteRunKompresja(vector<Uint8> input, int length);

/** ByteRun Decompression from a file
 * @param   fileName        name of a file to read a compressed input from
 */
vector<Uint8> ByteRunDekompresja(vector<Sint8> input);

/** RLE Compression and save to a file
 * @param   input       vector of Uint8 BW pixels to compress
 * @param   length      vector size
 * @param   fileName    name of a file to save compressed output to
 */
vector<Uint8> RLEKompresja(vector<Uint8> input, int length);

/** RLE Decompression from a file
 * @param   fileName        name of a file to read a compressed input from
 */
vector<Uint8> RLEDekompresja(vector<Uint8> input);

const int rozmiarBloku = 8;

struct macierz {
    float dct[rozmiarBloku][rozmiarBloku];
    Uint8 dane[rozmiarBloku][rozmiarBloku];
};

struct DCToutput {
    float mnoznik;
    vector<float> pierwszeWspolczynniki;
    vector<token8> reszta;
};

void wyswietlDane(macierz blok);

void wyswietlDCT(macierz blok);

macierz dct(Uint8 wartosci[rozmiarBloku][rozmiarBloku]);

macierz idct(float DCT[rozmiarBloku][rozmiarBloku]);

float findMaxABS(macierz blok, float lastMaxABS);

DCToutput DCTKompresja(Uint8 tryb);

void DCTDekompresja(string filename);

#endif //SM2024_PROJEKT_KOMPRESJA_H
