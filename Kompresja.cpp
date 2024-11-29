#include "Funkcje.h"
#include "Zmienne.h"
#include "Paleta.h"
#include "MedianCut.h"
#include "Pliki.h"
#include "Modele.h"
#include <math.h>
#include <list>
#include <vector>

using namespace std;


struct slowo{
    Uint16 kod =0;
    Uint8 dlugosc = 0;
    Uint8 element[4096];
    bool wSlowniku = false;
};

int rozmiarSlownika = 0;
slowo slownik[65535];

slowo noweSlowo(){
    slowo noweSlowo;
    noweSlowo.kod = 0;
    noweSlowo.dlugosc = 0;
    noweSlowo.wSlowniku = false;
    return noweSlowo;
}

slowo noweSlowo(Uint8 znak){
    slowo noweSlowo;
    noweSlowo.kod=0;
    noweSlowo.dlugosc=1;
    noweSlowo.element[0]=znak;
    noweSlowo.wSlowniku=false;
    return noweSlowo;
}

slowo polaczSlowo(slowo aktualneSlowo, Uint8 znak){
    slowo noweSlowo;

    if(aktualneSlowo.dlugosc<4096){
        noweSlowo.kod=0;
        noweSlowo.dlugosc = aktualneSlowo.dlugosc+1;
        noweSlowo.wSlowniku = false;
        copy(begin(aktualneSlowo.element), end(aktualneSlowo.element), begin(noweSlowo.element));
        noweSlowo.element[aktualneSlowo.dlugosc] = znak;
        return noweSlowo;
    }
    else{
        cout<<"UWAGA! przepelnienie rozmiaru znakow w pojedynczym slowie"<<endl;
        noweSlowo.kod =0;
        noweSlowo.dlugosc=0;
        noweSlowo.wSlowniku=false;
        noweSlowo.element[0] = znak;
        return noweSlowo;
    }
}

bool porownajSlowa(slowo slowo1, slowo slowo2){
    if(slowo1.dlugosc!=slowo2.dlugosc)
        return false;
    for(int s=0; s<slowo1.dlugosc; s++){
        if(slowo1.element[s] != slowo2.element[s])
            return false;
    }
    return true;
}

int znajdzWSlowniku(slowo szukany){
    for(int nr=0; nr<rozmiarSlownika; nr++)
        if(porownajSlowa(slownik[nr], szukany))
            return nr;
    return -1;
}

void wyswietlSlowo(slowo aktualneSlowo){
    if(aktualneSlowo.wSlowniku)
        cout<<"["<<aktualneSlowo.kod<<"] ";
    else
        cout<<"[X] ";
    for(int s=0; s<aktualneSlowo.dlugosc; s++){
        cout<<(int)aktualneSlowo.element[s];
        if(s<aktualneSlowo.dlugosc-1)
            cout<<", ";
    }
    cout<<endl;
}

int dodajDoSLownika(slowo nowy, bool czyWyswietlac = true){

    if(rozmiarSlownika <65536){
        Uint16 nr = rozmiarSlownika;
        slownik[nr].kod = nr;
        slownik[nr].dlugosc = nowy.dlugosc;

        copy(begin(nowy.element), end(nowy.element), begin(slownik[nr].element));
        slownik[nr].wSlowniku = true;
        if(czyWyswietlac)
            wyswietlSlowo(slownik[nr]);
        rozmiarSlownika++;
        return nr;
    }
    return -1;
}

void LZWinicjalizacja(){
    rozmiarSlownika = 0;

    for(int s=0; s<65536; s++){
        slownik[s].kod = 0;
        slownik[s].dlugosc=0;
        slownik[s].wSlowniku = false;
        memset(slownik[s].element, 0, sizeof(slownik[s].element));
    }

    slowo noweSlowo;
    for(int s=0; s<256; s++){
        noweSlowo.dlugosc =1;
        noweSlowo.element[0] = s;
        noweSlowo.kod = dodajDoSLownika(noweSlowo, false);
    }
}


void LZWKompresja(vector<Uint8> wejscie, int dlugosc, string fileName){
    cout << "Zapisywanie pliku z kompresją LZW..." << endl;
    LZWinicjalizacja();
    slowo aktualneSlowo = noweSlowo();
    slowo slowoZnak;
    Uint8 znak;
    int kod;
    int i=0;
    vector<Uint16> resultArr;
    while(i<dlugosc){

        znak=wejscie[i];
        slowoZnak = polaczSlowo(aktualneSlowo, znak);


        kod=znajdzWSlowniku(slowoZnak);

        if(kod<0){

            resultArr.push_back(aktualneSlowo.kod);
            dodajDoSLownika(slowoZnak, false);
            if(znajdzWSlowniku(slowoZnak)>0){
                slowoZnak.kod = znajdzWSlowniku(slowoZnak);

            }

            aktualneSlowo = noweSlowo(znak);
            aktualneSlowo.kod = znajdzWSlowniku(aktualneSlowo);
            aktualneSlowo.wSlowniku = true;
        }
        else{
            aktualneSlowo = slowoZnak;
            aktualneSlowo.kod = znajdzWSlowniku(aktualneSlowo);
            aktualneSlowo.wSlowniku = true;
        }

        i++;
    }

    resultArr.push_back(aktualneSlowo.kod);
    saveVector<Uint16>(resultArr, fileName);

}

void LZWDekompresja(string fileName) {
    vector<Uint16> skompresowane = readVector<Uint16>(fileName);
    int dlugosc = skompresowane.size();

    // Inicjalizacja słownika
    LZWinicjalizacja();

    vector<Uint8> wynik; // Wektor do przechowywania zdekompresowanego wyniku

    int poprzedniKod = skompresowane[0]; // Pierwszy kod w danych skompresowanych
    wynik.push_back(slownik[poprzedniKod].element[0]); // Dodaj pierwszy element do wyniku

    slowo poprzednieSlowo = slownik[poprzedniKod];
    slowo noweSlowo;

    for (int i = 1; i < dlugosc; i++) {
        int obecnyKod = skompresowane[i];
        slowo obecneSlowo;

        // Jeśli kod istnieje w słowniku
        if (obecnyKod < rozmiarSlownika) {
            obecneSlowo = slownik[obecnyKod];
        } else {
            // Kod nie istnieje w słowniku (specjalny przypadek)
            obecneSlowo = polaczSlowo(poprzednieSlowo, poprzednieSlowo.element[0]);
        }

        // Dodaj słowo do wyniku
        for (int j = 0; j < obecneSlowo.dlugosc; j++) {
            wynik.push_back(obecneSlowo.element[j]);
        }

        // Dodaj nowe słowo do słownika
        noweSlowo = polaczSlowo(poprzednieSlowo, obecneSlowo.element[0]);
        dodajDoSLownika(noweSlowo, false);

        // Ustaw obecne słowo jako poprzednie na następnej iteracji
        poprzednieSlowo = obecneSlowo;
    }

    // Wyświetlanie zdekompresowanego wyniku
//    cout << "Zdekompresowane dane:" << endl;
//    for (int x : wynik) {
//        cout << x << ", ";
//    }
//    cout << endl;
    int k = 0;
    SDL_Color pixel;
    for(int x = 0; x < szerokosc/2; x++){
        for(int y = 0; y < wysokosc/2; y++){
            pixel = z8BWna24RGB(wynik[k]);
            setPixel(x, y, pixel.r, pixel.g, pixel.b);
            k++;
        }
    }
}

void ByteRunKompresja(vector<Uint8> wejscie, int dlugosc, string fileName) {
    int i = 0;
    vector<Sint8> resultArr;

    while (i < dlugosc) {
        if ((i < dlugosc - 1) && (wejscie[i] == wejscie[i + 1])){
            int j = 0;
            while((i+  j < dlugosc - 1) && (wejscie[i + j] == wejscie[i + 1 + j]) && (j < 127)){
                j++;
            }

            resultArr.push_back(-j);
            resultArr.push_back(wejscie[i + j]);

            i += (j+1);
        } else {
            int j = 0;
            while((i + j < dlugosc - 1) && ((wejscie[i + j] != wejscie[i + j + 1])) && (j < 128)){
                j++;
            }

            if((i+j == dlugosc - 1) && (j < 128)){
                j++;
            }

            resultArr.push_back(j - 1);


            for(int k = 0; k < j; k++){
                resultArr.push_back(wejscie[i + k]);
            }

            i += j;
        }
    }

    saveVector<Sint8>(resultArr, fileName);
}


void ByteRunDekompresja(string fileName){
    int j = 0;
    vector<Sint8> wejscie = readVector<Sint8>(fileName);
    int dlugosc = wejscie.size();
    vector<Uint8> output;
    while (j < dlugosc){
        if(wejscie[j] < 0){
            int iters = (-1) * wejscie[j] + 1;
            j++;
            for(int i = 0; i < iters; i ++){
                output.push_back(wejscie[j]);
            }
        } else {
            int iters = wejscie[j] + 1;
            for(int i = 0; i < iters; i ++){
                j++;
                output.push_back(wejscie[j]);
            }
        }
        j++;
    }
    int k = 0;
    for(int x = 0; x < szerokosc/2; x++){
        for(int y = 0; y < wysokosc/2; y++){
            setPixel(x, y, output[k], output[k], output[k]);
            k++;
        }
    }
}


void RLEKompresja(vector<Uint8> wejscie, int dlugosc, string fileName){
    int i = 0;
    vector<Uint8> resultArr;

    while (i < dlugosc) {

        if ((i < dlugosc - 1) && (wejscie[i] == wejscie[i + 1])){

            int j = 0;
            while ((i + j < dlugosc - 1) && (wejscie[i + j] == wejscie[i + j + 1])
                   && (j < 254)){
                j++;
            }

            // cout<<"("<<(j + 1)<<"), " << wejscie[i+j]<<", ";

            resultArr.push_back((j+1));
            resultArr.push_back(wejscie[i+j]);

            i += (j+1);
        } else {

            int j = 0;

            while ((i + j < dlugosc - 1) && (wejscie[i + j] != wejscie[i + j + 1])
                   && (j < 254)){
                j++;
            }

            if( (i+j == dlugosc - 1) && (j < 254)){
                j++;
            }

            // cout << (int) 0 << ", " << j << ", ";
            resultArr.push_back((int)0);
            resultArr.push_back(j);

            for(int k = 0; k < j; k++){
                // cout << wejscie[i + k] << ", ";
                resultArr.push_back(wejscie[i + k]);
            }

            if (j % 2 != 0){
                // cout << (int) 0 << ", ";
                resultArr.push_back((int)0);
            }

            i += j;
        }
    }

    // cout << "Kompresja udana\n";
    saveVector<Uint8>(resultArr, fileName);
}

void RLEDekompresja(string fileName){
    int j = 0;
    vector<Uint8> wejscie = readVector<Uint8>(fileName);
    int dlugosc = wejscie.size();
    vector<Uint8> output;
    while (j < dlugosc){
        //// cout <<"W" << wejscie[j] << " ";
        if(wejscie[j] > 0){
            int iter = wejscie[j];
            // // cout <<"I" << iter << " ";
            j++;
            for(int i = 0; i < iter; i++){
                // cout << wejscie[j] << ", ";
                output.push_back(wejscie[j]);
            }
        } else if ( wejscie[j] == 0){
            j++;
            int iter = wejscie[j];
            for(int i = 0; i < iter; i++){
                j++;
                // cout << wejscie[j] << ", ";
                output.push_back(wejscie[j]);
            }

            if(iter % 2 != 0){
                j++;
            }
        }
        j++;
    }


    int k = 0;
    for(int x = 0; x < szerokosc/2; x++){
        for(int y = 0; y < wysokosc/2; y++){
            setPixel(x, y, output[k], output[k], output[k]);
            k++;
        }
    }
} 