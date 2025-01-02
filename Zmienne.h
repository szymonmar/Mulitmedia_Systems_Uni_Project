// zmienne globalne
#ifndef ZMIENNE_H_INCLUDED
#define ZMIENNE_H_INCLUDED

#include <SDL2/SDL.h>

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
/* * * * * * * * * * * PALLETTES * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * Pallette of all 24-bit RGB colors in the image
 */
extern SDL_Color paleta8[szerokosc*wysokosc];

/**
 * Number of colors in a pallette
 */
extern int ileKolorow;

/**
 * 8-bit RGB pallette
 */
extern SDL_Color paleta8k[256];

/**
 * 8-bit BW pallette
 */
extern SDL_Color paleta8s[256];

/**
 * 7-bit RGB pallette (SDL_Color) - used for saving to a file
 */
extern SDL_Color paleta7DoPliku[128];

/**
 * 7-bit RGB pallette (Uint8) - used for saving to a file
 */
extern Uint8 paleta7DoPlikuUint8[64000];

/**
 * 7-bit RGB pallette (SDL_Color) - used for Median Cut
 */
extern SDL_Color paleta7[128];

/**
 * Pallete size - used for saving pallette to a file
 */
extern Uint8 rozmiarPalety;

/**
 * Number of colors in a pallette and index of the last element
 * used for Median Cut
 */
extern int ileKubelkow;

/**
 * Pallette read from a file
 */
extern SDL_Color paleta[128];


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * *  MEDIAN CUT   * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * Array containing a whole image, used for
 * storing 7-bit RGB picked in 8x8 packets
 */
extern Uint8 obrazWTablicy[64000];

/**
 * Array used for Median Cut
 */
extern SDL_Color obrazek[64000];

/**
 * Filled with random pixels in Paleta > losujWartosci()
 * Not sure if still necessary
 */
extern SDL_Color obrazek0[64000];

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

/**
 * Dithering mode used for file saving / reading
 */
extern Uint8 dithering;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * *  FILTERING  * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * Two-dimensional array containing a whole image used for filtering
 */
extern int tabelaFiltr[320][200];

#endif // SM2024_ZMIENNE_H_INCLUDED
