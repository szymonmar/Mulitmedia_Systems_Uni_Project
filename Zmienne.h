// zmienne globalne
#ifndef ZMIENNE_H_INCLUDED
#define ZMIENNE_H_INCLUDED

#include <SDL2/SDL.h>
#include "Kompresja.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * SDL_COLOR AND WINDOW CONSTANTS / VARIABLES  * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * Window width in pixels
 */
#define szerokosc 640

/**
 * Window height in pixels
 */
#define wysokosc 400

/**
 * Window title
 */
#define tytul "SM2024 - Projekt - Zespol 21"

extern SDL_Window* window;
extern SDL_Surface* screen;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * *    FILES    * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * Image width in pixels used for file saving / reading
 */
extern Uint16 szerokoscObrazka;

/**
 * Image height in pixels used for file saving / reading
 */
extern Uint16 wysokoscObrazka;

/**
 * File ID
 */
extern char identyfikator[];

/**
 * Compression / filtering mode used file saving / reading
 */
extern Uint8 tryb;


extern DCToutput dctOutput;

extern vector<token16> LZ77output16;
extern vector<token8> LZ77output8;


#endif // SM2024_ZMIENNE_H_INCLUDED
