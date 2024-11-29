// główny plik projektu
#include "Zmienne.h"
#include "Funkcje.h"
#include "Paleta.h"
#include "MedianCut.h"
#include "Modele.h"

#include <exception>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
using namespace std;

#define tytul "SM2024 - Projekt - Zespol 21"

int main(int argc, char* argv[]) {


    cout << "\tMenu\n1.Obraz z paleta narzucona.\n2.Obraz w skali szarosci narzuconej\n3.Obraz w skali szarosci dedykowanej.\n4.Obraz z paleta wykryta\n5.Obraz z paleta dedykowana(MedianCut)\n6.Dithering\n7.Zapisz do pliku\n8.Odczytaj z pliku." << endl;



    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
    }

    window = SDL_CreateWindow(tytul, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szerokosc*2, wysokosc*2, SDL_WINDOW_SHOWN);

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
                    tryb = 1;
                    dithering = 0;
                }
                if (event.key.keysym.sym == SDLK_2){
                    Funkcja2();
                    tryb=2;
                    dithering = 0;
                }
                if (event.key.keysym.sym == SDLK_3){
                    Funkcja3();
                    tryb = 3;
                    dithering = 0;
                }
                if (event.key.keysym.sym == SDLK_4){
                    Funkcja4();
                    tryb = 4;
                    dithering = 0;
                }
                if (event.key.keysym.sym == SDLK_5){
                    Funkcja5();
                    tryb = 5;
                    dithering = 0;
                }
                if (event.key.keysym.sym == SDLK_6){
                    Funkcja6();
                    dithering = 1;
                }
                if (event.key.keysym.sym == SDLK_7)
                    Funkcja7();
                if (event.key.keysym.sym == SDLK_8)
                    Funkcja8();
                if (event.key.keysym.sym == SDLK_9)
                    Funkcja9();
                if (event.key.keysym.sym == SDLK_a)
                    ladujBMP("/Users/macbookpro/Desktop/SM/SM2024-Projekt/obrazek1.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_s)
                    ladujBMP("/Users/macbookpro/Desktop/SM/SM2024-Projekt/obrazek2.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_d)
                    ladujBMP("/Users/macbookpro/Desktop/SM/SM2024-Projekt/obrazek3.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_f)
                    ladujBMP("/Users/macbookpro/Desktop/SM/SM2024-Projekt/obrazek4.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_g)
                    ladujBMP("/Users/macbookpro/Desktop/SM/SM2024-Projekt/obrazek5.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_h)
                    ladujBMP("/Users/macbookpro/Desktop/SM/SM2024-Projekt/obrazek6.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_j)
                    ladujBMP("/Users/macbookpro/Desktop/SM/SM2024-Projekt/obrazek7.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_k)
                    ladujBMP("/Users/macbookpro/Desktop/SM/SM2024-Projekt/obrazek8.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_l)
                    ladujBMP("/Users/macbookpro/Desktop/SM/SM2024-Projekt/obrazek9.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_b)
                    czyscEkran(0, 0, 0);
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
