// funkcje do redukcji kolorów i tworzenia palet
#include "Paleta.h"
#include "Zmienne.h"
#include "Funkcje.h"

void czyscPalete() {
    ileKolorow = 0;
    for (int i = 0; i < 128; i++) {
        paleta7[i] = {0, 0, 0};
    }
}

int dodajKolor(SDL_Color kolor) {
    if (ileKolorow == 127) {
        //cout << "Paleta pełna" << endl;
        return -1;
    }
    int aktualnyKolor = ileKolorow;
    paleta7[aktualnyKolor] = kolor;
    //cout << aktualnyKolor << ": [" << (int)kolor.r <<", " << (int)kolor.g <<", " <<  (int)kolor.b << "]\n";
    ileKolorow++;
    return aktualnyKolor;
}

int sprawdzKolor(SDL_Color kolor) {
    if (ileKolorow > 0) {
        for(int i = 1; i < ileKolorow; i++) {
            //
            if (kolor.r == paleta7[i].r && kolor.g == paleta7[i].g && kolor.b == paleta7[i].b) {
                return i;
            }
        }
    }
    return dodajKolor(kolor);
}


void bubbleSort(SDL_Color arr[], int n){
    int i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = false;
        for (j = 0; j < n - i - 1; j++) {
            if ((int)arr[j].r > (int)arr[j + 1].r) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // If no two elements were swapped
        // by inner loop, then break
        if (swapped == false)
            break;
    }
}

void sortujKubelekKolor2(int start, int koniec, Uint8 sortowanie) {
    for(int i = start; i <= koniec; i++) {
        int maxI = i;
        for (int ii = i; ii <= koniec; ii++) {
            switch(sortowanie) {
                case 1:
                    if(obrazek[ii].r > obrazek[maxI].r) {
                        maxI = ii;
                    }
                    break;
                case 2:
                    if(obrazek[ii].g > obrazek[maxI].g) {
                        maxI = ii;
                    }
                    break;
                case 3:
                    if(obrazek[ii].b > obrazek[maxI].b) {
                        maxI = ii;
                    }
                    break;
            }
        }
        swap(obrazek[i], obrazek[maxI]);
    }
}


void losujWartosci()
{
    Uint8 wartosc;
    for (int i=0;i<256;i++)
    { wartosc = rand()%256;
        obrazek[i]={wartosc,wartosc,wartosc};
        obrazek0[i]={wartosc,wartosc,wartosc};
    }
}

void wyswietlWartosci()
{
    for (int i=0;i<256;i++){ cout <<(int)obrazek[i].r<<" ";}
    cout << endl;
}

int najwiekszaRoznica(int start, int koniec) {
    int minR = 0, minG = 0, minB = 0;
    int maxR = 0, maxG = 0, maxB = 0;
    Uint8 roznica = 0;
    for (int i = start; i <= koniec; i ++) {
        if (obrazek[i].b < obrazek[minB].b) {
            minB = i;
        }
        if (obrazek[i].r < obrazek[minB].r) {
            minR = i;
        }
        if (obrazek[i].g < obrazek[minB].g) {
            minG = i;
        }
        if (obrazek[i].b > obrazek[maxB].b) {
            maxB = i;
        }
        if (obrazek[i].r > obrazek[maxB].r) {
            maxR = i;
        }
        if (obrazek[i].g > obrazek[maxB].g) {
            maxG = i;
        }
    }
    int roznicaR = obrazek[maxR].r - obrazek[minR].r;
    int roznicaG = obrazek[maxR].g - obrazek[minR].g;
    int roznicaB = obrazek[maxR].b - obrazek[minR].b;
    int roznicaM = max(max(roznicaR, roznicaG), roznicaB);
    if (roznicaM == roznicaR) {
        return 1;
    }
    if (roznicaM == roznicaG) {
        return 2;
    }
    return 3;
}
void narysujPalete(SDL_Color paleta[])
{
    int index = 0;

    for (int y=wysokosc/2; y<wysokosc; y += 25){
        for (int x = 0; x<szerokosc/2; x += 20){
            for (int a=0; a<20; a++) {
                for (int b=0; b< 25; b++)
                {
                    setPixel(x + a, y + b, paleta[index].r, paleta[index].g, paleta[index].b);
                }
            }

            index++;
        }
    }
}
