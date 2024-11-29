#include <algorithm>
#include "MedianCut.h"
#include "Paleta.h"
#include "Zmienne.h"
#include "Funkcje.h"
#include "Modele.h"
using namespace std;

tabBayera oriTabBayera;

float normalize(int value) {
    return static_cast<float>(value) / 255.0f;
}

Uint8 normalize2(int val, int max, int min) {
    if(val > max) {
        val = max;
    } else if (val < min) {
        val = min;
    }
    return val;
}

int clamp(float value) {
    return std::max(0, std::min(255, static_cast<int>(value)));
}

YUV::YUV(Uint8 y, Uint8 u, Uint8 v) : Y(y), U(u), V(v) {};

YIQ::YIQ(Uint8 y, Uint8 i, Uint8 q) : Y(y), I(i), Q(q) {};
YCbCr::YCbCr(Uint8 y, Uint8 cb, Uint8 cr) : Y(y), Cb(cb), Cr(cr) {};
HSL::HSL(Uint8 h, Uint8 s, Uint8 l) : H(h), S(s), L(l) {};

SDL_Color YCbCrtoRGB(Uint8 y, Uint8 cb, Uint8 cr){

    SDL_Color RGBret;
    float r, g, b;

    r = y + 1.402 * (cr - 128);
    g = y - 0.344136 * (cb - 128) - 0.714136 * (cr - 128);
    b = y + 1.772 * (cb - 128);

    RGBret.r = (r<0 ? 0 : (r>255 ? 255 : r));
    RGBret.g = (g<0 ? 0 : (g>255 ? 255 : g));
    RGBret.b = (b<0 ? 0 : (b>255 ? 255 : b));

    return RGBret;
}

YCbCr RGBtoYCbCr(int xx, int yy){
    YCbCr YCbCrret;

    SDL_Color color = getPixel(xx, yy);

    YCbCrret.Y = 0 + (0.299 * color.r) + (0.587 * color.g) + (0.114 * color.b);
    YCbCrret.Cb = 128 - (0.168736 * color.r) - (0.331264 * color.g) + (0.5 * color.b);
    YCbCrret.Cr = 128 + (0.5 * color.r) - (0.418688 * color.g) - (0.081312 * color.b);

    return YCbCrret;
}

SDL_Color YUVtoRGB(Uint8 y, Uint8 u, Uint8 v){

    SDL_Color RGBret;
    float r, g, b;

    r = 1 * y + 0 * (u-128) + 1.401991 * (v - 128);
    g = 1 * y - 0.344135 * (u -128) - 0.714138 * (v - 128);
    b = 1 * y + 1.771999 * (u - 128) + 0 * (v - 128);

    RGBret.r = (r<0 ? 0 : (r>255 ? 255 : r));
    RGBret.g = (g<0 ? 0 : (g>255 ? 255 : g));
    RGBret.b = (b<0 ? 0 : (b>255 ? 255 : b));

    return RGBret;
}

YUV RGBtoYUV(int xx, int yy){

    YUV YUVret;

    SDL_Color color = getPixel(xx, yy);
    float y, u, v;

    y = (0.299 * color.r) + (0.587 * color.g) + (0.114 * color.b);
    u = 128 - (0.168736 * color.r) + (-0.331264 * color.g) + (0.5 * color.b);
    v = 128 + (0.5 * color.r) + (-0.418691 * color.g) + (-0.081309 * color.b);

    YUVret.Y = (y<0 ? 0 : (y>255 ? 255 : y));
    YUVret.U = (u<0 ? 0 : (u>255 ? 255 : u));
    YUVret.V = (v<0 ? 0 : (v>255 ? 255 : v));

    return YUVret;
}

SDL_Color YIQtoRGB(Uint8 y, Uint8 i, Uint8 q){

    SDL_Color RGBret;
    float r, g, b;

    r = y + ((i-128) * 1.1395) + ((q-128) * 0.647);
    g = y + ((i-128) * (-0.3242)) + ((q-128) * (-0.6762));
    b = y + ((i-128) * (-1.3182)) + ((q-128) * 1.78);

    RGBret.r = (r<0 ? 0 : (r>255 ? 255 : r));
    RGBret.g = (g<0 ? 0 : (g>255 ? 255 : g));
    RGBret.b = (b<0 ? 0 : (b>255 ? 255 : b));

    return RGBret;
}

YIQ RGBtoYIQ(int xx, int yy){

    YIQ YIQret;

    SDL_Color color = getPixel(xx, yy);
    float y, i, q;

    y = (0.299 * color.r) + (0.587 * color.g) + (0.114 * color.b);
    i = 128 + (0.5 * color.r) + (-0.23032 * color.g) + (-0.2696 * color.b);
    q = 128 + (0.2023 * color.r) + (-0.5 * color.g) + (0.2977 * color.b);

    YIQret.Y = (y<0 ? 0 : (y>255 ? 255 : y));
    YIQret.I = (i<0 ? 0 : (i>255 ? 255 : i));
    YIQret.Q = (q<0 ? 0 : (q>255 ? 255 : q));

    return YIQret;
}

SDL_Color HSLtoRGB(Uint8 h, Uint8 s, Uint8 l){

    SDL_Color RGBret;
    float zmienna1, zmienna2, barwa;
    float zmiennaR, zmiennaG, zmiennaB;

    float H = (h * 360.0) / 255.0;
    float S = s * 1.0 / 255.0;
    float L = l * 1.0 / 255.0;

    if(S == 0){
        RGBret.r = (L) * 255;
        RGBret.g = (L) * 255;
        RGBret.b = (L) * 255;
    } else {
        if (L < 0.5){
            zmienna1 = L * (1.0 + S);
        } else {
            zmienna1 = L + S - (L * S);
        }

        zmienna2 = 2 * L - zmienna1;
        barwa = H / 360;

        if (zmienna1 < 0) zmienna1 += 1;
        if (zmienna1 > 1) zmienna1 -= 1;

        if (zmienna2 < 0) zmienna2 += 1;
        if (zmienna2 > 1) zmienna2 -= 1;

        zmiennaR = barwa + 0.333;
        if (zmiennaR < 0) zmiennaR += 1;
        if (zmiennaR > 1) zmiennaR -= 1;

        zmiennaG = barwa;
        if (zmiennaG < 0) zmiennaG += 1;
        if (zmiennaG > 1) zmiennaG -= 1;

        zmiennaB = barwa - 0.333;
        if (zmiennaB < 0) zmiennaB += 1;
        if (zmiennaB > 1) zmiennaB -= 1;

        float czerwony;
        if((6 * zmiennaR) < 1){
            czerwony = zmienna2 + (zmienna1 - zmienna2) * 6 * zmiennaR;
        } else if((2 * zmiennaR) < 1 ){
            czerwony = zmienna1;
        } else if((3 * zmiennaR) < 2) {
            czerwony = zmienna2 + (zmienna1 - zmienna2) * (0.666 - zmiennaR) * 6;
        } else {
            czerwony = zmienna2;
        }

        float zielony;
        if((6 * zmiennaG) < 1){
            zielony = zmienna2 + (zmienna1 - zmienna2) * 6 * zmiennaG;
        } else if((2 * zmiennaG) < 1 ){
            zielony = zmienna1;
        } else if((3 * zmiennaG) < 2) {
            zielony = zmienna2 + (zmienna1 - zmienna2) * (0.666 - zmiennaG) * 6;
        } else {
            zielony = zmienna2;
        }

        float niebieski;
        if((6 * zmiennaB) < 1){
            niebieski = zmienna2 + (zmienna1 - zmienna2) * 6 * zmiennaB;
        } else if((2 * zmiennaB) < 1 ){
            niebieski = zmienna1;
        } else if((3 * zmiennaB) < 2) {
            niebieski = zmienna2 + (zmienna1 - zmienna2) * (0.666 - zmiennaB) * 6;
        } else {
            niebieski = zmienna2;
        }

        RGBret.r = czerwony * 255;
        RGBret.g = zielony * 255;
        RGBret.b = niebieski * 255;
    }

    return RGBret;
}

HSL RGBtoHSL(int xx, int yy){

    SDL_Color colorRGB = getPixel(xx, yy);
    float r, g, b, h, s, l, mmin, mmax;
    HSL HSLret;

    r = (float)colorRGB.r*1.0/255.0;
    g = (float)colorRGB.g*1.0/255.0;
    b = (float)colorRGB.b*1.0/255.0;

    mmin = std::min(r,(std::min(g,b)));
    mmax = std::max(r,(std::max(g,b)));

    l = (mmin+mmax)/2.0;

    if(mmin == mmax){
        s = 0;
    } else if(l <= 0.5){
        s = (mmax-mmin) / (mmax+mmin);
    } else{
        s = (mmax-mmin) / (2.0 - mmax - mmin);
    }

    if(r == mmax){
        h = (g - b) / (mmax-mmin);
    } else if(g == mmax){
        h = 2.0 + (b - r) / (mmax-mmin);
    } else{
        h = 4.0 + (r - g) / (mmax-mmin); // by³o r - b
    }

    h *= 60;
    if(h< 0) h += 360;

    h /= 360;
    h *= 255;
    s *= 255;
    l *= 255;

    HSLret.H = (int)h;
    HSLret.S = (int)s;
    HSLret.L = (int)l;

    return HSLret;
}




tabBayera zaktualizujTabBayera4(float zakres, int rozmiar)
{
    tabBayera zakTabBayera;
    float podzial = zakres*1.0 / (rozmiar*rozmiar);
    for(int y=0; y<rozmiar; y++)
    {
        for(int x=0; x<rozmiar; x++)
        {
            zakTabBayera.tabBayera4[y][x] = (oriTabBayera.tabBayera4[y][x]*podzial)-podzial/2;
        }
    }
    return zakTabBayera;
};

SDL_Color getRGB555_(int xx, int yy) {
    SDL_Color pixel = getPixel(xx, yy);
    float R, G, B;
    R = pixel.r;
    G = pixel.g;
    B = pixel.b;
    R = R * 31.0 / 255.0;
    G = G * 31.0 / 255.0;
    B = B * 31.0 / 255.0;
    SDL_Color newColor;
    newColor.r = R;
    newColor.g = G;
    newColor.b = B;
    return newColor;
}

Uint16 getRGB555(int xx, int yy) {
    SDL_Color pixel = getPixel(xx, yy);
    Uint16 R, G, B, RR, GG, BB;
    R = pixel.r;
    G = pixel.g;
    B = pixel.b;
    Uint16 ret;

    R <<= 7;
    G <<= 2;
    B >>= 3;

    RR = R & 0b0111110000000000;
    GG = G & 0b0000001111100000;
    BB = B & 0b0000000000011111;
    ret = RR + GG + BB;
    return ret;
}

SDL_Color getRGB565_(int xx, int yy) {
    SDL_Color pixel = getPixel(xx, yy);
    float R, G, B;
    R = pixel.r;
    G = pixel.g;
    B = pixel.b;
    R = R * 31.0 / 255.0;
    G = G * 63.0 / 255.0;
    B = B * 31.0 / 255.0;
    SDL_Color newColor;
    newColor.r = R;
    newColor.g = G;
    newColor.b = B;
    return newColor;
}

Uint16 getRGB565(int xx, int yy) {
    SDL_Color pixel = getPixel(xx, yy);
    Uint16 R, G, B, RR, GG, BB;
    R = pixel.r;
    G = pixel.g;
    B = pixel.b;
    Uint16 ret;

    R <<= 8;
    G <<= 3;
    B >>= 3;

    RR = R & 0b1111100000000000;
    GG = G & 0b0000011111100000;
    BB = B & 0b0000000000011111;
    ret = RR + GG + BB;
    return ret;
}
void setRGB555(int xx, int yy, Uint8 r, Uint8 g, Uint8 b) {
    Uint8 R, G, B;
    R = r * 255 / 31;
    G = g * 255 / 31;
    B = b * 255 / 31;
    setPixel(xx, yy, R, G, B);
}
void setRGB565(int xx, int yy, Uint8 r, Uint8 g, Uint8 b) {
    Uint8 R, G, B;
    R = r * 255 / 31;
    G = g * 255 / 63;
    B = b * 255 / 31;
    setPixel(xx, yy, R, G, B);
}
void setRGB555(int xx, int yy, Uint16 rgb555) {
    Uint16 R, G, B;
    Uint16 RR, GG, BB;
    R = rgb555;
    G = rgb555;
    B = rgb555;

    RR = R & 0b0111110000000000;
    GG = G & 0b0000001111100000;
    BB = B & 0b0000000000011111;

    RR >>= 7;
    GG >>= 2;
    BB <<= 3;
    setPixel(xx, yy, (Uint8)RR, (Uint8)GG, (Uint8)BB);
}
void setRGB565(int xx, int yy, Uint16 rgb565) {
    Uint16 R, G, B;
    Uint16 RR, GG, BB;
    R = rgb565;
    G = rgb565;
    B = rgb565;

    RR = R & 0b1111100000000000;
    GG = G & 0b0000011111100000;
    BB = B & 0b0000000000011111;

    RR >>= 8;
    GG >>= 3;
    BB <<= 3;
    setPixel(xx, yy, (Uint8)RR, (Uint8)GG, (Uint8)BB);
}

SDL_Color dithering565(int x, int y) {
    tabBayera tabBayera565RB = zaktualizujTabBayera4(32.0, 4);
    tabBayera tabBayera565G = zaktualizujTabBayera4(64.0, 4);
    SDL_Color kolor;
    int rozmiar = 4;
    float tablicaRB, tablicaG;
    float zakR, zakG, zakB;
    int R, G, B;

    kolor = getRGB565_(x,y);
    tablicaRB = tabBayera565RB.tabBayera4[y % rozmiar][x % rozmiar];
    tablicaG = tabBayera565G.tabBayera4[y % rozmiar][x % rozmiar];

    zakR = kolor.r+1;
    zakG = kolor.g+1;
    zakB = kolor.b+1;



    if(kolor.r > tablicaRB){
        R = floor(zakR*8.25);
    } else {
        R = floor((zakR-1)*8.25);
    }

    if(kolor.g > tablicaG){
        G = floor(zakG*4.12);
    } else {
        G = floor((zakG-1)*4.12);
    }

    if(kolor.b > tablicaRB){
        B = floor(zakB*8.25);
    } else {
        B = floor((zakB-1)*8.25);
    }
    kolor.r = normalize2(R, 255, 0);
    kolor.g = normalize2(G, 255, 0);
    kolor.b = normalize2(B, 255, 0);

    return kolor;

}

SDL_Color dithering555(int x, int y) {
    tabBayera tabBayera555 = zaktualizujTabBayera4(32.0, 4);
    SDL_Color kolor;
    int rozmiar = 4;
    float tablica;
    float zakR, zakG, zakB;
    int R, G, B;

    kolor = getRGB555_(x,y);
    tablica = tabBayera555.tabBayera4[y % rozmiar][x % rozmiar];

    zakR = kolor.r+1;
    zakG = kolor.g+1;
    zakB = kolor.b+1;



    if(kolor.r > tablica){
        R = floor(zakR*8.25);
    } else {
        R = floor((zakR-1)*8.25);
    }

    if(kolor.g > tablica){
        G = floor(zakG*8.25);
    } else {
        G = floor((zakG-1)*8.25);
    }

    if(kolor.b > tablica){
        B = floor(zakB*8.25);
    } else {
        B = floor((zakB-1)*8.25);
    }
    kolor.r = normalize2(R, 255, 0);
    kolor.g = normalize2(G, 255, 0);
    kolor.b = normalize2(B, 255, 0);

    return kolor;

}

pixele4HSL HSL_420(pixele4HSL pixele4, char param) {
    float avg;
    pixele4HSL ret = pixele4;
    if(param == 'h') {
        avg = (pixele4.a.H + pixele4.b.H + pixele4.c.H + pixele4.d.H) / 4;
        ret.a.H = ret.b.H = ret.c.H = ret.d.H = avg;
        return ret;
    } else if (param == 's') {
        avg = (pixele4.a.S + pixele4.b.S + pixele4.c.S + pixele4.d.S) / 4;
        ret.a.S = ret.b.S = ret.c.S = ret.d.S = avg;
        return ret;
    } else if (param == 'l') {
        avg = (pixele4.a.L + pixele4.b.L + pixele4.c.L + pixele4.d.L) / 4;
        ret.a.L = ret.b.L = ret.c.L = ret.d.L = avg;
        return ret;
    } else {
        cout << "HSL_420: Unknown param: " << param << endl;
        return ret;
    }
}

pixele4YUV YUV_420(pixele4YUV pixele4, char param) {
    float avg;
    pixele4YUV ret = pixele4;
    if(param == 'y') {
        avg = (pixele4.a.Y + pixele4.b.Y + pixele4.c.Y + pixele4.d.Y) / 4;
        ret.a.Y = ret.b.Y = ret.c.Y = ret.d.Y = avg;
        return ret;
    } else if (param == 'u') {
        avg = (pixele4.a.U + pixele4.b.U + pixele4.c.U + pixele4.d.U) / 4;
        ret.a.U = ret.b.U = ret.c.U = ret.d.U = avg;
        return ret;
    } else if (param == 'v') {
        avg = (pixele4.a.V + pixele4.b.V + pixele4.c.V + pixele4.d.V) / 4;
        ret.a.V = ret.b.V = ret.c.V = ret.d.V = avg;
        return ret;
    } else {
        cout << "YUV_420: Unknown param: " << param << endl;
        return ret;
    }
}

pixele4YIQ YIQ_420(pixele4YIQ pixele4, char param) {
    float avg;
    pixele4YIQ ret = pixele4;
    if(param == 'y') {
        avg = (pixele4.a.Y + pixele4.b.Y + pixele4.c.Y + pixele4.d.Y) / 4;
        ret.a.Y = ret.b.Y = ret.c.Y = ret.d.Y = avg;
        return ret;
    } else if (param == 'i') {
        avg = (pixele4.a.I + pixele4.b.I + pixele4.c.I + pixele4.d.I) / 4;
        ret.a.I = ret.b.I = ret.c.I = ret.d.I = avg;
        return ret;
    } else if (param == 'q') {
        avg = (pixele4.a.Q + pixele4.b.Q + pixele4.c.Q + pixele4.d.Q) / 4;
        ret.a.Q = ret.b.Q = ret.c.Q = ret.d.Q = avg;
        return ret;
    } else {
        cout << "YIQ_420: Unknown param: " << param << endl;
        return ret;
    }
}

pixele4RGB RGB_420(pixele4RGB pixele4, char param) {
    float avg;
    pixele4RGB ret = pixele4;
    if(param == 'r') {
        avg = (pixele4.a.r + pixele4.b.r + pixele4.c.r + pixele4.d.r) / 4;
        ret.a.r = ret.b.r = ret.c.r = ret.d.r = avg;
        return ret;
    } else if (param == 'g') {
        avg = (pixele4.a.g + pixele4.b.g + pixele4.c.g + pixele4.d.g) / 4;
        ret.a.g = ret.b.g = ret.c.g = ret.d.g = avg;
        return ret;
    } else if (param == 'b') {
        avg = (pixele4.a.b + pixele4.b.b + pixele4.c.b + pixele4.d.b) / 4;
        ret.a.b = ret.b.b = ret.c.b = ret.d.b = avg;
        return ret;
    } else {
        cout << "RGB_420: Unknown param: " << param << endl;
        return ret;
    }
}

pixele4YCbCr YCbCr_420(pixele4YCbCr pixele4, char param) {
    float avg;
    pixele4YCbCr ret = pixele4;
    if(param == 'y') {
        avg = (pixele4.a.Y + pixele4.b.Y + pixele4.c.Y + pixele4.d.Y) / 4;
        ret.a.Y = ret.b.Y = ret.c.Y = ret.d.Y = avg;
        return ret;
    } else if (param == 'b') {
        avg = (pixele4.a.Cb + pixele4.b.Cb + pixele4.c.Cb + pixele4.d.Cb) / 4;
        ret.a.Cb = ret.b.Cb = ret.c.Cb = ret.d.Cb = avg;
        return ret;
    } else if (param == 'r') {
        avg = (pixele4.a.Cr + pixele4.b.Cr + pixele4.c.Cr + pixele4.d.Cr) / 4;
        ret.a.Cr = ret.b.Cr = ret.c.Cr = ret.d.Cr = avg;
        return ret;
    } else {
        cout << "YUV_420: Unknown param: " << param << endl;
        return ret;
    }
}
