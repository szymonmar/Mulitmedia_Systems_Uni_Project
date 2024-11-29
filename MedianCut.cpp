// metody do algorytmu kwantyzacji (MedianCut)
#include "MedianCut.h"
#include "Paleta.h"
#include "Zmienne.h"
#include "Funkcje.h"

using namespace std;

void medianCutBW(int start, int koniec, int iteracja)
{
    if (iteracja >0) {
        bubbleSort(obrazek,256);

        int srodek = (start + koniec + 1)/2;

        medianCutBW(start,srodek-1,iteracja-1);
        medianCutBW(srodek,koniec,iteracja-1);

    }
    else
    {
        int sumaBW = 0;
        for (int p=start; p<=koniec;p++)
        {
            sumaBW += obrazek[p].r;
        }
        Uint8 nowyBW = sumaBW/ (koniec+1 - start);
        SDL_Color nowyKolor = {nowyBW, nowyBW, nowyBW};
        paleta7[ileKubelkow] = nowyKolor;
        ileKubelkow++;
    }
}
void medianCut(int start, int koniec, int iteracja)
{

    if (iteracja > 0) {
        int sortowanie = najwiekszaRoznica(start, koniec);
        sortujKubelekKolor2(start, koniec, sortowanie);
        int srodek = (start + koniec + 1) / 2;

        medianCut(start,srodek-1,iteracja-1);
        medianCut(srodek,koniec,iteracja-1);

    }
    else
    {
        int sumaR = 0, sumaG = 0, sumaB = 0;
        for (int i = start; i <= koniec; i++) {
            sumaR += obrazek[i].r;
            sumaG += obrazek[i].g;
            sumaB += obrazek[i].b;
        }
        SDL_Color nowyKolor{(Uint8)(sumaR/(koniec+1-start)), (Uint8)(sumaG/(koniec+1-start)), (Uint8)(sumaB/(koniec+1-start))};
        paleta7[ileKubelkow] = nowyKolor;
        ileKubelkow++;
    }
}

int znajdzSasiadaBW(Uint8 wartosc)
{

    int minimum = 999;
    int indexMinimum = 0;

    int odleglosc = 0;

    for (int i=0;i<127;i++)
    {
        odleglosc = abs(wartosc - paleta7[i].r);
        if (odleglosc < minimum) {minimum = odleglosc; indexMinimum = i;}
    }
    return indexMinimum;
}

int znajdzSasiada(SDL_Color kolor)
{
    //cout << "znajdz sasiada " << endl;
    int minimum = 9999;
    int indexMinimum = 0;
    SDL_Color kolorPaleta;
    int odleglosc = 0;
    for (int i=0;i<127;i++)
    {
        kolorPaleta = paleta7[i];
        odleglosc = abs(kolor.r - kolorPaleta.r) + abs(kolor.g - kolorPaleta.g) + abs(kolor.b - kolorPaleta.b);
        if (odleglosc < minimum) {minimum = odleglosc; indexMinimum = i;}
    }
    //cout << "sasiad: " << indexMinimum <<  endl;
    return indexMinimum;
}
