// metody do algorytmu kwantyzacji (MedianCut)
#ifndef MEDIANCUT_H_INCLUDED
#define MEDIANCUT_H_INCLUDED

#include <SDL2/SDL.h>

void medianCutBW(int start, int koniec, int iteracja);
void medianCut(int start, int koniec, int iteracja);

/**
 * Searches the pallette (paleta7) for the closest
 * color to the one passed in the parameter (Uint8 BW)
 * @param kolor color to find the closest one to in the pallette
 * @return index of the closest color in the pallette (paleta7)
 */
int znajdzSasiadaBW(Uint8 wartosc);

/**
 * Searches the pallette (paleta7) for the closest
 * color to the one passed in the parameter (SDL_Color RGB)
 * @param kolor color to find the closest one to in the pallette
 * @return index of the closest color in the pallette (paleta7)
 */
int znajdzSasiada(SDL_Color kolor);

#endif // SM2024_MEDIANCUT_H_INCLUDED
