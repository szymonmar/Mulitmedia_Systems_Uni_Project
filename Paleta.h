// funkcje do redukcji kolorów i tworzenia palet
#ifndef PALETA_H_INCLUDED
#define PALETA_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
using namespace std;

/**
 * Clears the pallette (paleta7)
 */
void czyscPalete();

/**
 * Adds new color to the pallette (paleta7)
 * @param kolor new color
 * @return index of the new color in the pallette or -1 if the pallette is full
 */
int dodajKolor(SDL_Color kolor);

/**
 * Checks if color is in the pallette & if not adds it
 * @param kolor color to look for
 * @return index of the color in the pallette or -1 if the pallette is full
 */
int sprawdzKolor(SDL_Color kolor);

/**
 * Sorts SDL_Color array based on red value
 * @param arr   array to sort
 * @param n     max index
 */
void bubbleSort(SDL_Color arr[], int n);

/**
 * Sorts specific range of obrazek array
 * @param start         first index in range
 * @param koniec        last index in range
 * @param sortowanie    sorting mode ( 1 - RED, 2 - GREEN, 3 - BLUE )
 */
void sortujKubelekKolor2(int start, int koniec, Uint8 sortowanie);

/**
 * Generates random grayscale colors (Uint8)
 * and fills obrazek & obrazek0 arrays with them
 * (arrays are the same)
 */
void losujWartosci();

/**
 * Prints grayscale colors from obrazek array to the console
 */
void wyswietlWartosci();

/**
 * Finds RGB component with the biggest difference between the
 * hightest and the lowest value in the specific range of obrazek array
 * @param start     start index in obrazek
 * @param koniec    end index in obrazek
 * @return  1 - RED,
 *          2 - GREEN,
 *          3 - BLUE
 */
int najwiekszaRoznica(int start, int koniec);

/**
 * Draws pallette in second row, first column
 * @param paleta pallette to draw (max 128 colors)
 */
void narysujPalete(SDL_Color paleta[]);

#endif // SM2024_PALETA_H_INCLUDED
