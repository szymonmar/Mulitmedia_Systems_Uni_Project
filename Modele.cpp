#include <algorithm>
#include "Funkcje.h"
#include "Modele.h"
using namespace std;

tabBayera oriTabBayera;

Uint8 normalize(int val, int max, int min) {
    if(val > max) {
        val = max;
    } else if (val < min) {
        val = min;
    }
    return val;
}

HSL::HSL(Uint8 h, Uint8 s, Uint8 l) : H(h), S(s), L(l) {};

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

void setRGB565(int xx, int yy, Uint8 r, Uint8 g, Uint8 b) {
    Uint8 R, G, B;
    R = r * 255 / 31;
    G = g * 255 / 63;
    B = b * 255 / 31;
    setPixel(xx, yy, R, G, B);
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
    kolor.r = normalize(R, 255, 0);
    kolor.g = normalize(G, 255, 0);
    kolor.b = normalize(B, 255, 0);

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
