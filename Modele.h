#ifndef MODELE_H_INCLUDED
#define MODELE_H_INCLUDED
#include <SDL2/SDL.h>
#include <algorithm>
using namespace std;

float normalize(int value);
Uint8 normalize2(int val, int max, int min);
int clamp(float value);
struct YUV {
    Uint8 Y, U, V;
    YUV(Uint8 y = 0, Uint8 u = 0, Uint8 v = 0);
};


struct YIQ {
    Uint8 Y, I, Q;
    YIQ(Uint8 y = 0, Uint8 i = 0, Uint8 q = 0);
};



struct YCbCr {
    Uint8 Y, Cb, Cr;
    YCbCr(Uint8 y = 0, Uint8 cb = 0, Uint8 cr = 0);
};

struct HSL{
    Uint8 H, S, L;
    HSL(Uint8 h = 0, Uint8 s = 0, Uint8 l = 0);
};
SDL_Color YCbCrtoRGB(Uint8 y, Uint8 cb, Uint8 cr);

YCbCr RGBtoYCbCr(int xx, int yy);

SDL_Color YUVtoRGB(Uint8 y, Uint8 u, Uint8 v);

YUV RGBtoYUV(int xx, int yy);

SDL_Color YIQtoRGB(Uint8 y, Uint8 i, Uint8 q);

YIQ RGBtoYIQ(int xx, int yy);

SDL_Color HSLtoRGB(Uint8 h, Uint8 s, Uint8 l);

HSL RGBtoHSL(int xx, int yy);

void setRGB555(int xx, int yy, Uint8 r, Uint8 g, Uint8 b);
void setRGB555(int xx, int yy, Uint16 rgb555) ;
void setRGB565(int xx, int yy, Uint8 r, Uint8 g, Uint8 b);
void setRGB565(int xx, int yy, Uint16 rgb565) ;
SDL_Color getRGB555_(int xx, int yy);
Uint16 getRGB555(int xx, int yy) ;
SDL_Color getRGB565_(int xx, int yy);
Uint16 getRGB565(int xx, int yy);


struct tabBayera {
    int tabBayera4[4][4] = {{6, 14, 8, 16},
                            {10, 2, 12, 4},
                            {7, 15, 5, 13},
                            {11, 3, 9, 1}};
};



tabBayera zaktualizujTabBayera4(float zakres, int rozmiar);

SDL_Color getRGB555_(int xx, int yy);
Uint16 getRGB555(int xx, int yy);
SDL_Color getRGB565_(int xx, int yy);
Uint16 getRGB565(int xx, int yy);
void setRGB555(int xx, int yy, Uint8 r, Uint8 g, Uint8 b);
void setRGB565(int xx, int yy, Uint8 r, Uint8 g, Uint8 b) ;
void setRGB555(int xx, int yy, Uint16 rgb555);
void setRGB565(int xx, int yy, Uint16 rgb565);
SDL_Color dithering565(int x, int y);
SDL_Color dithering555(int x, int y);


struct pixele4YUV {
    YUV a;
    YUV b;
    YUV c;
    YUV d;
};

struct pixele4YIQ {
    YIQ a;
    YIQ b;
    YIQ c;
    YIQ d;
};

struct pixele4YCbCr {
    YCbCr a;
    YCbCr b;
    YCbCr c;
    YCbCr d;
};

struct pixele4RGB {
    SDL_Color a;
    SDL_Color b;
    SDL_Color c;
    SDL_Color d;
};

struct pixele4HSL {
    HSL a;
    HSL b;
    HSL c;
    HSL d;
};

pixele4HSL HSL_420(pixele4HSL pixele4, char param);
pixele4YUV YUV_420(pixele4YUV pixele4, char param);
pixele4YIQ YIQ_420(pixele4YIQ pixele4, char param);
pixele4RGB RGB_420(pixele4RGB pixele4, char param);
pixele4YCbCr YCbCr_420(pixele4YCbCr pixele4, char param);

#endif
