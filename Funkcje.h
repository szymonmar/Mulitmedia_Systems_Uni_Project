// podstawowe funkcje
#ifndef FUNKCJE_H_INCLUDED
#define FUNKCJE_H_INCLUDED

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

/*
 * Main program functions, mapped with 1-9 keyboard buttons
 */
void Funkcja1();
void Funkcja2();
void Funkcja3();
void Funkcja4();
void Funkcja5();
void Funkcja6();
void Funkcja7();
void Funkcja8();
void Funkcja9();

/* YUV - U & V subsampling
 */
void podprobkowanieYUV();

/* YIQ - I & Q subsampling
 */
void podprobkowanieYIQ();

/* YCbCr - Cb & Cr subsampling
 */
void podprobkowanieYCBCR();

/* HSL - L subsampling
 */
void podprobkowanieHSL();

/* Converts 24-bit RGB pixel to 7-bit RGB (RRGGGBB0)
 * @param   kolor   input pixel
 * @return  7-bit RGB pixel in Uint8
 */
Uint8 z24RGBna7RGB(SDL_Color kolor);

/* Converts 7-bit RGB pixel (RRGGGBB0) to 24-bit RGB SDL_Color
 * @param   kolor7bit   input pixel
 * @return  24-bit RGB pixel in SDL_Color
 */
SDL_Color z7RGBna24RGB(Uint8 kolor7bit);

/* Converts 24-bit RGB pixel to 7-bit BW
 * @param   kolor   input pixel
 * @return  7-bit BW pixel in Uint8
 */
Uint8 z24RGBna7BW (SDL_Color kolor);

/* Converts 7-bit BW pixel to 24-bit RGB SDL_Color
 * @param   szary7bit   input pixel
 * @return  24-bit RGB pixel in SDL_Color
 */
SDL_Color z7BWna24RGB (Uint8 szary7bit);

/* Converts 24-bit RGB pixel to 8-bit BW
 * @param   kolor   input pixel
 * @return  8-bit BW pixel in Uint8
 */
Uint8 z24RGBna8BW (SDL_Color kolor);

/* Converts 8-bit BW pixel to 24-bit RGB SDL_Color
 * @param   szary8bit   input pixel
 * @return  24-bit RGB pixel in SDL_Color
 */
SDL_Color z8BWna24RGB (Uint8 szary8bit);

/* Reads pixels from an image in 8x8 packets,
 * converts to 7-bit RGB and saves to obrazWTablicy[]
 */
void zbierzKoloryZObrazka();

/* Sets certain color to a pixel in the window
 * @param   x   X coordinate of the pixel
 * @param   y   Y coordinate of the pixel
 * @param   R   value of red color in RGB model
 * @param   G   value of green color in RGB model
 * @param   B   value of blue color in RGB model
 */
void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);

/* Reads pixel data from screen
 * @param   x   X coordinate of a pixel
 * @param   y   Y coordinate of a pixel
 */
SDL_Color getPixel(int x, int y);

/* Fills screen with a certain color (default: black)
 * @param   R   value of red in RGB
 * @param   G   value of green in RGB
 * @param   B   value of blue in RGB
 */
void czyscEkran(Uint8 R = 0, Uint8 G = 0, Uint8 B = 0);

/* Reads BMP file and displays it in the front end (size 320 x 200)
 * @param   nazwa   absolute path to the file
 * @param   x       X coordinate of a top left corner of the picture
 * @param   y       Y coordinate of a top left corner of the picture
 */
void ladujBMP(char const* nazwa, int x, int y);

/* Performs a sub-filtering algorithm on an image
 * @param   tabela      image stored in a two-dimensional array
 */
void filtrRoznicowy(Uint8 tabela[320][200]);

/* Performs a up-filtering algorithm on an image
 * @param   tabela      image stored in a two-dimensional array
 */
void roznicaLinii(Uint8 tabela[320][200]);

/* Performs an average filtering algorithm on an image
 * @param   tabela      image stored in a two-dimensional array
 */
void filtrUsredniajacy(Uint8 tabela[320][200]);

/* Performs a Paeth filtering algorithm on an image
 * @param   tabela      image stored in a two-dimensional array
 */
void filtrPaetha(Uint8 tabela[320][200]);

#endif // SM2024_FUNKCJE_H_INCLUDED
