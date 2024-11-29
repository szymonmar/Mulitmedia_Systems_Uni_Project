// zmienne globalne
#ifndef ZMIENNE_H_INCLUDED
#define ZMIENNE_H_INCLUDED

#include <SDL2/SDL.h>

#define szerokosc 640
#define wysokosc 400

#define tytul "SM2024 - Projekt - Zespol 21"

extern SDL_Window* window;
extern SDL_Surface* screen;

extern SDL_Color paleta8[szerokosc*wysokosc];
extern int ileKolorow;

extern SDL_Color paleta8k[256];
extern SDL_Color paleta8s[256];

extern SDL_Color obrazek[320*200];
extern SDL_Color obrazek0[320*200];

extern Uint16 szerokoscObrazka;
extern Uint16 wysokoscObrazka;

extern SDL_Color paleta7DoPliku[128];
extern Uint8 paleta7DoPlikuUint8[szerokosc/2*wysokosc/2];

extern SDL_Color paleta7[128];

extern Uint8 rozmiarPalety;
extern int ileKubelkow;

extern SDL_Color paleta[128];

extern char identyfikator[];
extern Uint8 tryb;
extern Uint8 dithering;

extern Uint8 obrazWTablicy[64000];

extern int tabelaFiltr[320][200];

#endif // SM2024_ZMIENNE_H_INCLUDED
