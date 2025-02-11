#ifndef MODELE_H_INCLUDED
#define MODELE_H_INCLUDED
#include <SDL2/SDL.h>
#include <algorithm>
using namespace std;

/**
 * Normalizes given value not to exceed given min-max
 * @param val value to normalize
 * @param max min wanted value
 * @param min max wanted value
 * @return normalized value
 */
Uint8 normalize(int val, int max, int min);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * *    MODELS   * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/** Single pixel in HSL model
 * @param   H   Hue
 * @param   S   Saturation
 * @param   L   Ligthness
 */
struct HSL{
    Uint8 H, S, L;
    HSL(Uint8 h = 0, Uint8 s = 0, Uint8 l = 0);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * *    CONVERTERS   * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * YCbCr to SDL_Color RGB Converter
 * @param h Hue
 * @param s Saturation
 * @param l Luma
 * @return 24-bit RGB SDL_Color
 */
SDL_Color HSLtoRGB(Uint8 h, Uint8 s, Uint8 l);

/**
 * SDL_Color RGB to HSL Converter
 * @param xx X coordinate of a pixel
 * @param yy Y coordinate of a pixel
 * @return HSL struct
 */
HSL RGBtoHSL(int xx, int yy);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * *    GETTERS & SETTERS    * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * Sets RGB565 pixel to the front-end
 * @param xx    X coordinate of the pixel
 * @param yy    Y coordinate of the pixel
 * @param r     5-bit R value
 * @param g     6-bit G value
 * @param b     5-bit B value
 */
void setRGB565(int xx, int yy, Uint8 r, Uint8 g, Uint8 b);

/**
 * Sets RGB565 pixel to the front-ent
 * @param xx        X coordinate of the pixel
 * @param yy        Y coordinate of the pixel
 * @param rgb565    RGB565 as Uint16 value (RRRRR GGGGGG BBBBB)
 */
void setRGB565(int xx, int yy, Uint16 rgb565) ;

/**
 * Gets pixel from the front-end and converts it to RGB565
 * @param xx X coordinate of the pixel to convert
 * @param yy Y coordinate of the pixel to convert
 * @return RGB565 pixel in SDL_Color
 */
SDL_Color getRGB565_(int xx, int yy);

/**
 * Gets pixel from the front-end and converts it to RGB565
 * @param xx X coordinate of the pixel to convert
 * @param yy Y coordinate of the pixel to convert
 * @return RGB565 pixel in Uint16 (RRRRR GGGGGG BBBBB)
 */
Uint16 getRGB565(int xx, int yy);



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * *    DITHERING    * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 * Bayer array used for dithering
 */
struct tabBayera {
    int tabBayera4[4][4] = {{6, 14, 8, 16},
                            {10, 2, 12, 4},
                            {7, 15, 5, 13},
                            {11, 3, 9, 1}};
};

/**
 * Fills Bayer's array with values from a certain range
 * @param zakres    maximum value in the array
 * @param rozmiar   array size (number of rows or columns)
 * @return          updated array
 */
tabBayera zaktualizujTabBayera4(float zakres, int rozmiar);

/**
 * Gets pixel from the front-end, converts to RGB565
 * and performs dithering
 * @param x     X coordinate of the pixel
 * @param y     Y coordinate of the pixel
 * @return SDL_Color pixel after dithering (ready to display)
 */
SDL_Color dithering565(int x, int y);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * *     SUBSAMPLING     * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * Packet of four HSL pixels (used for subsampling)
 */
struct pixele4HSL {
    HSL a;
    HSL b;
    HSL c;
    HSL d;
};

/**
 * Averages one component using a packet of four HSL pixels (subsampling)
 * @param pixele4   packet of four HSL pixels
 * @param param     component ( 'h' / 's' / 'l' )
 * @return          packet of four HSL pixels after subsampling
 */
pixele4HSL HSL_420(pixele4HSL pixele4, char param);

#endif
