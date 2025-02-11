// zmienne globalne
#include "Zmienne.h"

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;

Uint16 szerokoscObrazka = 320;
Uint16 wysokoscObrazka = 200;

char identyfikator[] = "ESP";
Uint8 tryb = 0;

DCToutput dctOutput;

vector<token16> LZ77output16;
vector<token8> LZ77output8;

