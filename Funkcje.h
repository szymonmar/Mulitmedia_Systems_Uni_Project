// podstawowe funkcje
#ifndef FUNKCJE_H_INCLUDED
#define FUNKCJE_H_INCLUDED

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

/**
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
void Funkcja10();

/** Converts 24-bit RGB pixel to 8-bit BW
 * @param   kolor   input pixel
 * @return  8-bit BW pixel in Uint8
 */
Uint8 z24RGBna8BW (SDL_Color kolor);

/** Converts 8-bit BW pixel to 24-bit RGB SDL_Color
 * @param   szary8bit   input pixel
 * @return  24-bit RGB pixel in SDL_Color
 */
SDL_Color z8BWna24RGB (Uint8 szary8bit);

/** Reads pixels from an image in 8x8 packets,
 * converts to 7-bit RGB and saves to obrazWTablicy[]
 */
void zbierzKoloryZObrazka();

/** Sets certain color to a pixel in the window
 * @param   x   X coordinate of the pixel
 * @param   y   Y coordinate of the pixel
 * @param   R   value of red color in RGB model
 * @param   G   value of green color in RGB model
 * @param   B   value of blue color in RGB model
 */
void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);

/** Reads pixel data from screen
 * @param   x   X coordinate of a pixel
 * @param   y   Y coordinate of a pixel
 */
SDL_Color getPixel(int x, int y);

/** Fills screen with a certain color (default: black)
 * @param   R   value of red in RGB
 * @param   G   value of green in RGB
 * @param   B   value of blue in RGB
 */
void czyscEkran(Uint8 R = 0, Uint8 G = 0, Uint8 B = 0);

/** Reads BMP file and displays it in the front end (size 320 x 200)
 * @param   nazwa   absolute path to the file
 * @param   x       X coordinate of a top left corner of the picture
 * @param   y       Y coordinate of a top left corner of the picture
 */
void ladujBMP(char const* nazwa, int x, int y);

/** Performs a sub-filtering algorithm on an image
 * @param   input      image stored in an Uint8 vector
 */
vector<Uint8> filtrRoznicowy(vector<Uint8> input);

/** Reverses sub-filtering
 * @param   input      image stored in an Uint8 vector
 */
vector<Uint8> reverseFiltrRoznicowy(vector<Uint8> input);

/** Performs a sub-filtering algorithm on an image
 * @param   input      image stored in an Uint16 vector
 */
vector<Uint16> filtrRoznicowy(vector<Uint16> input);

/** Reverses sub-filtering
 * @param   input      image stored in an Uint16 vector
 */
vector<Uint16> reverseFiltrRoznicowy(vector<Uint16> input);

#endif // SM2024_FUNKCJE_H_INCLUDED
