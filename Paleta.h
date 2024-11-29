// funkcje do redukcji kolorów i tworzenia palet
#ifndef PALETA_H_INCLUDED
#define PALETA_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
using namespace std;

void czyscPalete();
int dodajKolor(SDL_Color kolor);
int sprawdzKolor(SDL_Color kolor);
void bubbleSort(SDL_Color arr[], int n);
void sortujKubelekKolor2(int start, int koniec, Uint8 sortowanie);
void losujWartosci();
void wyswietlWartosci();
int najwiekszaRoznica(int start, int koniec);
void narysujPalete(SDL_Color paleta[]);


#endif // SM2024_PALETA_H_INCLUDED
