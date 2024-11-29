// podstawowe funkcje
#ifndef FUNKCJE_H_INCLUDED
#define FUNKCJE_H_INCLUDED

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <SDL2/SDL.h>

using namespace std;


void Funkcja1();
void Funkcja2();
void Funkcja3();
void Funkcja4();
void Funkcja5();
void Funkcja6();
void Funkcja7();
void Funkcja8();
void Funkcja9();


void podprobkowanieYUV();
void podprobkowanieYIQ();
void podprobkowanieYCBCR();
void podprobkowanieHSL();
Uint8 z24RGBna7RGB(SDL_Color kolor);
SDL_Color z7RGBna24RGB(Uint8 kolor7bit);
Uint8 z24RGBna7BW (SDL_Color kolor);
SDL_Color z7BWna24RGB (Uint8 szary7bit);
Uint8 z24RGBna8BW (SDL_Color kolor);
SDL_Color z8BWna24RGB (Uint8 szary7bit);
void zbierzKoloryZObrazka();
void byteRunDekompresja(string a);
void byteRunKompresja(Uint8 wejscie[], int dlugosc, string a);


void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel(int x, int y);
void czyscEkran(Uint8 R, Uint8 G, Uint8 B);
void ladujBMP(char const* nazwa, int x, int y);
void filtrRoznicowy(Uint8 tabela[320][200]);
void roznicaLinii(Uint8 tabela[320][200]);
void filtrUsredniajacy(Uint8 tabela[320][200]);
void filtrPaetha(Uint8 tabela[320][200]);

#endif // SM2024_FUNKCJE_H_INCLUDED
