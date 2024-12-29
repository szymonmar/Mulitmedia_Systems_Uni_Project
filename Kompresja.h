#ifndef SM2024_PROJEKT_KOMPRESJA_H
#define SM2024_PROJEKT_KOMPRESJA_H

#include "Modele.h"
#include <vector>


using namespace std;

/* Stuct representing single word, used in LZW compression & decompression
 */
struct slowo{
    Uint16 kod =0;
    Uint8 length = 0;
    Uint8 element[4096];
    bool wSlowniku = false;
};

/* Default constructor for slowo struct,
 * creating an empty slowo
 */
slowo noweSlowo();

/* Constructor for slowo struct,
 * creating a slowo with one element
 * @param   znak        starting element for a slowo
 */
slowo noweSlowo(Uint8 znak);

/* For adding elements to slowo struct
 * @param   aktualneSlowo       slowo struct
 * @param   znak                element to add
 * @return  slowo               slowo struct with a new element
 */
slowo polaczSlowo(slowo aktualneSlowo, Uint8 znak);

/* For comparing two words (slowo structs)
 * @param   slowo1      1st word
 * @param   slowo2      2nd word
 * @return  true        if words are identical
 *          false       if words are not identical
 */
bool porownajSlowa(slowo slowo1, slowo slowo2);

/* Finds words in dictionary
 * @param   szukany     word to look for
 * @return  index of a word     if the word exists
 *          -1                  if word not found
 */
int znajdzWSlowniku(slowo szukany);

/* Prints word to a console
 * @param   aktualneSlowo   word to print
 */
void wyswietlSlowo(slowo aktualneSlowo);

/* Adds a word to a dictionary
 * @param   slowo           word to add
 * @param   czyWyswietlac   true to print the added word
 *                          false not to print
 *                          (default: false)
 * @return  index of the word in a dictionary
 *          -1 if dictionary overflow
 */
int dodajDoSLownika(slowo nowy, bool czyWyswietlac = false);

/* Init function for LZW compression, sets up the dictionary
 * runs always before compression / decompression
 */
void LZWinicjalizacja();

/* LZW Compression and save to file
 * @param   input       vector of Uint8 BW pixels to compress
 * @param   length      vector size
 * @param   fileName    name of a file to save compressed output to
 */
void LZWKompresja(vector<Uint8> input, int length, string fileName);

/* LZW Decompression from a file
 * @param   fileName        name of a file to read compressed input from
 */
void LZWDekompresja(string fileName);

/* LZ77 Compression and save to file
 * @param   input       vector of Uint8 BW pixels to compress
 * @param   length      vector size
 * @param   filename    name of a file to save compressed output to
 */
void LZ77Kompresja(vector<Uint8> input, int length, string filename);

/* LZ77 Decompression from a file
 * @param   filename    name of a file to read a compressed input from
 */
void LZ77Dekompresja(string filename);

/* ByteRun Compression and save to a file
 * @param   input       vector of Uint8 BW pixels to compress
 * @param   length      vector size
 * @param   fileName    name of a file to save compressed output to
 */
void ByteRunKompresja(vector<Uint8> input, int length, string fileName);

/* ByteRun Decompression from a file
 * @param   fileName        name of a file to read a compressed input from
 */
void ByteRunDekompresja(string fileName);

/* RLE Compression and save to a file
 * @param   input       vector of Uint8 BW pixels to compress
 * @param   length      vector size
 * @param   fileName    name of a file to save compressed output to
 */
void RLEKompresja(vector<Uint8> input, int length, string fileName);

/* RLE Decompression from a file
 * @param   fileName        name of a file to read a compressed input from
 */
void RLEDekompresja(string fileName);

#endif //SM2024_PROJEKT_KOMPRESJA_H
