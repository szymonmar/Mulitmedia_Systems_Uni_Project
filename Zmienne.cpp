// zmienne globalne
#include "Zmienne.h"

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;


SDL_Color paleta8[szerokosc*wysokosc];
int ileKolorow = 0;

SDL_Color paleta8k[256];
SDL_Color paleta8s[256];

SDL_Color obrazek[64000];
SDL_Color obrazek0[64000];

Uint16 szerokoscObrazka = szerokosc;
Uint16 wysokoscObrazka = wysokosc;

SDL_Color paleta7DoPliku[128];
Uint8 paleta7DoPlikuUint8[64000];

SDL_Color paleta7[128];

Uint8 rozmiarPalety = 0;
int ileKubelkow;

SDL_Color paleta[128];

char identyfikator[] = "ESPJ";
Uint8 tryb = 0;
Uint8 dithering = 0;

Uint8 obrazWTablicy[64000];

int tabelaFiltr[320][200];

