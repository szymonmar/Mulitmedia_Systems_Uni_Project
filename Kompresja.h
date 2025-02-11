#ifndef SM2024_PROJEKT_KOMPRESJA_H
#define SM2024_PROJEKT_KOMPRESJA_H

#include "Modele.h"
#include <vector>
using namespace std;

/**
 * Output token struct for LZ77 compression, uses 8-bit raw value
 * (best for 8-bit B&W, RGB888 or HSL888 models)
 */
struct token8 {
    Uint16 tokLength;
    Uint16 shift;
    Uint8 rawValue;
    token8() : tokLength(0), shift(0), rawValue(0) {}
    token8(Uint16 tokLength, Uint16 shift, Uint8 rawValue): shift(shift), tokLength(tokLength), rawValue(rawValue) {};
};

/**
 * Output token struct for LZ77 compression, uses 16-bit raw value
 * (best for RGB565 model)
 */
struct token16 {
    Uint16 tokLength;
    Uint16 shift;
    Uint16 rawValue;
    token16() : tokLength(0), shift(0), rawValue(0) {}
    token16(Uint16 tokLength, Uint16 shift, Uint16 rawValue): shift(shift), tokLength(tokLength), rawValue(rawValue) {};
};

/**
 * LZ77 Compression for Uint8 input
 * @param input     vector of Uint8 values to compress
 * @param length    input vector length
 * @return          vector of token8 structures
 */
vector<token8> LZ77Kompresja(vector<Uint8> input, int length);

/**
 * LZ77 Decompression for Uint8 output
 * @param tokens    vector of token8 structures to decompress
 * @return          vector of Uint8 values
 */
vector<Uint8> LZ77Dekompresja(vector<token8> tokens);

/**
 * LZ77 Compression for Uint16 input
 * @param input     vector of Uint16 values to compress
 * @param length    input vector length
 * @return          vector of token16 structures
 */
vector<token16> LZ77Kompresja(vector<Uint16> input, int length);

/**
 * LZ77 Decompression for Uint16 output
 * @param tokens    vector of token16 structures to decompress
 * @return          vector of Uint16 values
 */
vector<Uint16> LZ77Dekompresja(vector<token16> tokens);

/** RLE Compression
 * @param   input       vector of Uint8 pixels to compress
 * @param   length      vector size
 */
vector<Uint8> RLEKompresja(vector<Uint8> input, int length);

/** RLE Decompression
 * @param   input       vector of Uint8 values to decompress
 */
vector<Uint8> RLEDekompresja(vector<Uint8> input);

/**
 * Block size for DCT
 */
const int rozmiarBloku = 8;

/**
 * Struct used to perform DCT
 */
struct macierz {
    float dct[rozmiarBloku][rozmiarBloku];
    Uint8 dane[rozmiarBloku][rozmiarBloku];
};

/**
 * Output struct for DCT algorithm, contains all the data to save to a file
 * float mnoznik - multiplier for quantisation
 * vector<float> pierwszeWspolczynniki - first elements of each DCT matrix
 * vector<Uint8> reszta - rest of the DCT matrices elements
 */
struct DCToutput {
    float mnoznik;
    vector<float> pierwszeWspolczynniki;
    vector<Uint8> reszta;
};

/**
 * Performs dct algorithm on a block of data
 * @param wartosci block of Uint8 data
 * @return macierz struct
 */
macierz dct(Uint8 wartosci[rozmiarBloku][rozmiarBloku]);

/**
 * Reverses dct algorithm on a block of data
 * @param DCT DCT matrix
 * @return macierz struct
 */
macierz idct(float DCT[rozmiarBloku][rozmiarBloku]);

/**
 * Finds max absolute value of a macierz struct / many macierz structs
 * @param blok macierz struct
 * @param lastMaxABS last found max absolute value to compare
 * @return max absolute value
 */
float findMaxABS(macierz blok, float lastMaxABS);

/**
 * Reads an image from the left side of the front-end and compresses the
 * whole image using DTC algorithm, then compressing DTC matrices elements
 * using RLE algorithm
 * @param tryb image mode
 * @return DTCoutput struct
 */
DCToutput DCTKompresja(Uint8 tryb);

/**
 * Decompresses DTCoutput to a vector of matrices of size rozmiarBloku x rozmiarBloku
 * Decompressed vector contains:
 * matrices of 8-bit BW values (if tryb says image is b&w)
 * or
 * matrices of 8-bit color factors, first all values of red, then all values of blue, then all values of green etc.
 * @param input input struct
 * @param tryb image mode
 * @return
 */
vector<macierz> DCTDekompresja(DCToutput input, Uint8 tryb);

#endif //SM2024_PROJEKT_KOMPRESJA_H
