// główny plik projektu
#include "Zmienne.h"
#include "Funkcje.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <filesystem>
#include <SDL2/SDL.h>
using namespace std;

#define tytul "SM2024 - Projekt - Zespol 21"

int main(int argc, char* argv[]) {
    tryb = 0b11000000;
    int x = 1;
    int y = 0;
    int inc = 1;
    int limit = 1;

    cout << "\nUWAGA!\nW celu uzyskania optymalnego rozmiaru pliku oraz"
            "\nzachowania wysokiej jakości obrazu wywołuj funkcje zgodnie"
            "\nz kolejnością w menu głównym";

    cout << "\n\n\tMenu"
            "\n1.Wybór trybu COLOR / BW"
            "\n2.Wybór trybu 16-bit / 24-bit"
            "\n3.Wybór modelu barwnego RGB888 / HSL888"
            "\n4.Dithering"
            "\n5.Predykcja (filtr różnicowy)"
            "\n6.Kompresja bezstratna (LZ77)"
            "\n7.Kompresja stratna (Podpróbkowanie 420)"
            "\n8.Kompresja stratna (DCT)"
            "\n9.Zapis do pliku."
            "\n0.Odczyt z pliku." << endl;



    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
    }

    window = SDL_CreateWindow(tytul, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szerokosc*2, wysokosc, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    screen = SDL_GetWindowSurface(window);
    if (screen == NULL) {
        fprintf(stderr, "SDL_GetWindowSurface Error: %s\n", SDL_GetError());
    return false;
    }
    SDL_UpdateWindowSurface(window);

    string nazwa;
    bool done = false;
    SDL_Event event;
    // główna pętla programu
    while (SDL_WaitEvent(&event)) {

        // sprawdzamy czy pojawiło się zdarzenie
        switch (event.type) {
            case SDL_QUIT:
                done = true;
                break;

            // sprawdzamy czy został wciśnięty klawisz
            case SDL_KEYDOWN: {
                // wychodzimy, gdy wciśnięto ESC
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    done = true;
                if (event.key.keysym.sym == SDLK_1){
                    Funkcja1();
                }
                if (event.key.keysym.sym == SDLK_2){
                    Funkcja2();
                }
                if (event.key.keysym.sym == SDLK_3){
                    Funkcja3();
                }
                if (event.key.keysym.sym == SDLK_4){
                    Funkcja4();
                }
                if (event.key.keysym.sym == SDLK_5){
                    Funkcja5();
                }
                if (event.key.keysym.sym == SDLK_6){
                    Funkcja6();
                }
                if (event.key.keysym.sym == SDLK_7)
                    Funkcja7();
                if (event.key.keysym.sym == SDLK_8)
                    Funkcja8();
                if (event.key.keysym.sym == SDLK_9)
                    Funkcja9();
                if (event.key.keysym.sym == SDLK_0)
                    Funkcja10();
                if (event.key.keysym.sym == SDLK_a) {
                    ladujBMP("/obrazek1.bmp", 0, 0);
                    tryb = 0b11000000;
                }
                if (event.key.keysym.sym == SDLK_s) {
                    ladujBMP("/obrazek2.bmp", 0, 0);
                    tryb = 0b11000000;
                }
                if (event.key.keysym.sym == SDLK_d) {
                    ladujBMP("/obrazek3.bmp", 0, 0);
                    tryb = 0b11000000;
                }
                if (event.key.keysym.sym == SDLK_f) {
                    ladujBMP("/obrazek4.bmp", 0, 0);
                    tryb = 0b11000000;
                }
                if (event.key.keysym.sym == SDLK_g) {
                    ladujBMP("/obrazek5.bmp", 0, 0);
                    tryb = 0b11000000;
                }
                if (event.key.keysym.sym == SDLK_h) {
                    ladujBMP("/obrazek6.bmp", 0, 0);
                    tryb = 0b11000000;
                }
                if (event.key.keysym.sym == SDLK_j) {
                    ladujBMP("/obrazek7.bmp", 0, 0);
                    tryb = 0b11000000;
                }
                if (event.key.keysym.sym == SDLK_k) {
                    ladujBMP("/obrazek8.bmp", 0, 0);
                    tryb = 0b11000000;
                }
                if (event.key.keysym.sym == SDLK_l) {
                    ladujBMP("/obrazek9.bmp", 0, 0);
                    tryb = 0b11000000;
                }
                if (event.key.keysym.sym == SDLK_b) {
                    czyscEkran(0, 0, 0);
                    tryb = 0b11000000;
                }
                else
                    break;
               }
        }
        if (done) break;
    }

    if (screen) {
        SDL_FreeSurface(screen);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }


    SDL_Quit();
    return 0;
}
