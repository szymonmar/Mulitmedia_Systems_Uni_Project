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

/** Single pixel in YUV model
 * @param   Y   Luma
 * @param   U   Chroma (blue)
 * @param   V   Chroma (red)
 */
struct YUV {
    Uint8 Y, U, V;
    YUV(Uint8 y = 0, Uint8 u = 0, Uint8 v = 0);
};

/** Single pixel in YIQ model
 * @param   Y   Luma
 * @param   I   Chroma (orange-blue)
 * @param   Q   Chroma (purple-green)
 */
struct YIQ {
    Uint8 Y, I, Q;
    YIQ(Uint8 y = 0, Uint8 i = 0, Uint8 q = 0);
};

/** Single pixel in YCbCr model
 * @param   Y   Luma
 * @param   Cb  Chroma (counter-blue)
 * @param   Cr  Chroma (counter-red)
 */
struct YCbCr {
    Uint8 Y, Cb, Cr;
    YCbCr(Uint8 y = 0, Uint8 cb = 0, Uint8 cr = 0);
};

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
 * @param y Luma
 * @param cb Chroma (counter-blue)
 * @param cr Chroma (counter-red)
 * @return 24-bit RGB SDL_Color
 */
SDL_Color YCbCrtoRGB(Uint8 y, Uint8 cb, Uint8 cr);

/**
 * SDL_Color RGB to YCbCr Converter
 * @param xx X coordinate of a pixel
 * @param yy Y coordinate of a pixel
 * @return YCbCr struct
 */
YCbCr RGBtoYCbCr(int xx, int yy);

/**
 * YUV to SDL_Color RGB Converter
 * @param y Luma
 * @param u Chroma (blue)
 * @param v Chroma (red)
 * @return 24-bit RGB SDL_Color
 */
SDL_Color YUVtoRGB(Uint8 y, Uint8 u, Uint8 v);

/**
 * SDL_Color RGB to YUV Converter
 * @param xx X coordinate of a pixel
 * @param yy Y coordinate of a pixel
 * @return YUV struct
 */
YUV RGBtoYUV(int xx, int yy);

/**
 * YIQ to SDL_Color RGB Converter
 * @param y Luma
 * @param i Chroma (orange-blue)
 * @param q Chroma (purple-green)
 * @return 24-bit RGB SDL_Color
 */
SDL_Color YIQtoRGB(Uint8 y, Uint8 i, Uint8 q);

/**
 * SDL_Color RGB to YIQ Converter
 * @param xx X coordinate of a pixel
 * @param yy Y coordinate of a pixel
 * @return YIQ struct
 */
YIQ RGBtoYIQ(int xx, int yy);

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
 * Sets RGB555 pixel to the front-end
 * @param xx    X coordinate of the pixel
 * @param yy    Y coordinate of the pixel
 * @param r     5-bit R value
 * @param g     5-bit G value
 * @param b     5-bit B value
 */
void setRGB555(int xx, int yy, Uint8 r, Uint8 g, Uint8 b);

/**
 * Sets RGB555 pixel to the front-ent
 * @param xx        X coordinate of the pixel
 * @param yy        Y coordinate of the pixel
 * @param rgb555    RGB555 as Uint16 value (0 RRRRR GGGGG BBBBB)
 */
void setRGB555(int xx, int yy, Uint16 rgb555) ;

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
 * Gets pixel from the front-end and converts it to RGB555
 * @param xx X coordinate of the pixel to convert
 * @param yy Y coordinate of the pixel to convert
 * @return RGB555 pixel in SDL_Color
 */
SDL_Color getRGB555_(int xx, int yy);

/**
 * Gets pixel from the front-end and converts it to RGB555
 * @param xx X coordinate of the pixel to convert
 * @param yy Y coordinate of the pixel to convert
 * @return RGB555 pixel in Uint16 (0 RRRRR GGGGG BBBBB)
 */
Uint16 getRGB555(int xx, int yy) ;

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

/**
 * Gets pixel from the front-end, converts to RGB555
 * and performs dithering
 * @param x     X coordinate of the pixel
 * @param y     Y coordinate of the pixel
 * @return SDL_Color pixel after dithering (ready to display)
 */
SDL_Color dithering555(int x, int y);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * *     SUBSAMPLING     * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * Packet of four YUV pixels (used for subsampling)
 */
struct pixele4YUV {
    YUV a;
    YUV b;
    YUV c;
    YUV d;
};

/**
 * Packet of four YIQ pixels (used for subsampling)
 */
struct pixele4YIQ {
    YIQ a;
    YIQ b;
    YIQ c;
    YIQ d;
};

/**
 * Packet of four YCbCr pixels (used for subsampling)
 */
struct pixele4YCbCr {
    YCbCr a;
    YCbCr b;
    YCbCr c;
    YCbCr d;
};

/**
 * Packet of four RGB pixels (used for subsampling)
 */
struct pixele4RGB {
    SDL_Color a;
    SDL_Color b;
    SDL_Color c;
    SDL_Color d;
};

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

/**
 * Averages one component using a packet of four YUV pixels (subsampling)
 * @param pixele4   packet of four YUV pixels
 * @param param     component ( 'y' / 'u' / 'v' )
 * @return          packet of four YUV pixels after subsampling
 */
pixele4YUV YUV_420(pixele4YUV pixele4, char param);

/**
 * Averages one component using a packet of four YIQ pixels (subsampling)
 * @param pixele4   packet of four YIQ pixels
 * @param param     component ( 'y' / 'i' / 'q' )
 * @return          packet of four YIQ pixels after subsampling
 */
pixele4YIQ YIQ_420(pixele4YIQ pixele4, char param);

/**
 * Averages one component using a packet of four RGB pixels (subsampling)
 * @param pixele4   packet of four RGB pixels
 * @param param     component ( 'r' / 'g' / 'b' )
 * @return          packet of four RGB pixels after subsampling (SDL_Color)
 */
pixele4RGB RGB_420(pixele4RGB pixele4, char param);

/**
 * Averages one component using a packet of four YCbCr pixels (subsampling)
 * @param pixele4   packet of four YCbCr pixels
 * @param param     component ( 'y' / 'b' / 'r' )
 * @return          packet of four YCbCr pixels after subsampling
 */
pixele4YCbCr YCbCr_420(pixele4YCbCr pixele4, char param);

#endif
