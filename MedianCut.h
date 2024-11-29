// metody do algorytmu kwantyzacji (MedianCut)
#ifndef MEDIANCUT_H_INCLUDED
#define MEDIANCUT_H_INCLUDED

#include <SDL2/SDL.h>

void medianCutBW(int start, int koniec, int iteracja);
void medianCut(int start, int koniec, int iteracja);
int znajdzSasiadaBW(Uint8 wartosc);
int znajdzSasiada(SDL_Color kolor);

#endif // SM2024_MEDIANCUT_H_INCLUDED
