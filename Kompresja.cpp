#include "Funkcje.h"
#include "Zmienne.h"
#include "Paleta.h"
#include "MedianCut.h"
#include "Pliki.h"
#include "Modele.h"
#include "Kompresja.h"
#include <math.h>
#include <list>
#include <vector>

using namespace std;

int rozmiarSlownika = 0;
slowo slownik[65535];

slowo noweSlowo(){
    slowo noweSlowo;
    noweSlowo.kod = 0;
    noweSlowo.length = 0;
    noweSlowo.wSlowniku = false;
    return noweSlowo;
}

slowo noweSlowo(Uint8 znak){
    slowo noweSlowo;
    noweSlowo.kod=0;
    noweSlowo.length=1;
    noweSlowo.element[0]=znak;
    noweSlowo.wSlowniku=false;
    return noweSlowo;
}

slowo polaczSlowo(slowo aktualneSlowo, Uint8 znak){
    slowo noweSlowo;

    if(aktualneSlowo.length<4096){
        noweSlowo.kod=0;
        noweSlowo.length = aktualneSlowo.length+1;
        noweSlowo.wSlowniku = false;
        copy(begin(aktualneSlowo.element), end(aktualneSlowo.element), begin(noweSlowo.element));
        noweSlowo.element[aktualneSlowo.length] = znak;
        return noweSlowo;
    }
    else{
        cout<<"UWAGA! przepelnienie rozmiaru znakow w pojedynczym slowie"<<endl;
        noweSlowo.kod =0;
        noweSlowo.length=0;
        noweSlowo.wSlowniku=false;
        noweSlowo.element[0] = znak;
        return noweSlowo;
    }
}

bool porownajSlowa(slowo slowo1, slowo slowo2){
    if(slowo1.length!=slowo2.length)
        return false;
    for(int s=0; s<slowo1.length; s++){
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
    for(int s=0; s<aktualneSlowo.length; s++){
        cout<<(int)aktualneSlowo.element[s];
        if(s<aktualneSlowo.length-1)
            cout<<", ";
    }
    cout<<endl;
}

int dodajDoSLownika(slowo nowy, bool czyWyswietlac){

    if(rozmiarSlownika <65536){
        Uint16 nr = rozmiarSlownika;
        slownik[nr].kod = nr;
        slownik[nr].length = nowy.length;

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
        slownik[s].length=0;
        slownik[s].wSlowniku = false;
        memset(slownik[s].element, 0, sizeof(slownik[s].element));
    }

    slowo noweSlowo;
    for(int s=0; s<256; s++){
        noweSlowo.length =1;
        noweSlowo.element[0] = s;
        noweSlowo.kod = dodajDoSLownika(noweSlowo, false);
    }
}


vector<Uint16> LZWKompresja(vector<Uint8> input, int length){
    cout << "Zapisywanie pliku z kompresją LZW..." << endl;
    LZWinicjalizacja();
    slowo aktualneSlowo = noweSlowo();
    slowo slowoZnak;
    Uint8 znak;
    int kod;
    int i=0;
    vector<Uint16> resultArr;
    while(i<length){

        znak=input[i];
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

    return resultArr;
}

vector<Uint8> LZWDekompresja(vector<Uint16> skompresowane) {
    //vector<Uint16> skompresowane = readVector<Uint16>(fileName);
    int length = skompresowane.size();

    // Inicjalizacja słownika
    LZWinicjalizacja();

    vector<Uint8> wynik; // Wektor do przechowywania zdekompresowanego wyniku

    int poprzedniKod = skompresowane[0]; // Pierwszy kod w danych skompresowanych
    wynik.push_back(slownik[poprzedniKod].element[0]); // Dodaj pierwszy element do wyniku

    slowo poprzednieSlowo = slownik[poprzedniKod];
    slowo noweSlowo;

    for (int i = 1; i < length; i++) {
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
        for (int j = 0; j < obecneSlowo.length; j++) {
            wynik.push_back(obecneSlowo.element[j]);
        }

        // Dodaj nowe słowo do słownika
        noweSlowo = polaczSlowo(poprzednieSlowo, obecneSlowo.element[0]);
        dodajDoSLownika(noweSlowo, false);

        // Ustaw obecne słowo jako poprzednie na następnej iteracji
        poprzednieSlowo = obecneSlowo;
    }

    return wynik;
}

vector<Sint8> ByteRunKompresja(vector<Uint8> input, int length) {
    int i = 0;
    vector<Sint8> resultArr;

    while (i < length) {
        if ((i < length - 1) && (input[i] == input[i + 1])){
            int j = 0;
            while((i+  j < length - 1) && (input[i + j] == input[i + 1 + j]) && (j < 127)){
                j++;
            }

            resultArr.push_back(-j);
            resultArr.push_back(input[i + j]);

            i += (j+1);
        } else {
            int j = 0;
            while((i + j < length - 1) && ((input[i + j] != input[i + j + 1])) && (j < 128)){
                j++;
            }

            if((i+j == length - 1) && (j < 128)){
                j++;
            }

            resultArr.push_back(j - 1);


            for(int k = 0; k < j; k++){
                resultArr.push_back(input[i + k]);
            }

            i += j;
        }
    }
    return resultArr;
}

vector<Uint8> ByteRunDekompresja(vector<Sint8> input){
    int j = 0;
    //vector<Sint8> input = readVector<Sint8>(fileName);
    int length = input.size();
    vector<Uint8> output;
    while (j < length){
        if(input[j] < 0){
            int iters = (-1) * input[j] + 1;
            j++;
            for(int i = 0; i < iters; i ++){
                output.push_back(input[j]);
            }
        } else {
            int iters = input[j] + 1;
            for(int i = 0; i < iters; i ++){
                j++;
                output.push_back(input[j]);
            }
        }
        j++;
    }
    return output;
}

vector<Uint8> RLEKompresja(vector<Uint8> input, int length){
    int i = 0;
    vector<Uint8> resultArr;

    while (i < length) {

        if ((i < length - 1) && (input[i] == input[i + 1])){

            int j = 0;
            while ((i + j < length - 1) && (input[i + j] == input[i + j + 1])
                   && (j < 254)){
                j++;
            }

            // cout<<"("<<(j + 1)<<"), " << input[i+j]<<", ";

            resultArr.push_back((j+1));
            resultArr.push_back(input[i+j]);

            i += (j+1);
        } else {

            int j = 0;

            while ((i + j < length - 1) && (input[i + j] != input[i + j + 1])
                   && (j < 254)){
                j++;
            }

            if( (i+j == length - 1) && (j < 254)){
                j++;
            }

            // cout << (int) 0 << ", " << j << ", ";
            resultArr.push_back((int)0);
            resultArr.push_back(j);

            for(int k = 0; k < j; k++){
                // cout << input[i + k] << ", ";
                resultArr.push_back(input[i + k]);
            }

            if (j % 2 != 0){
                // cout << (int) 0 << ", ";
                resultArr.push_back((int)0);
            }

            i += j;
        }
    }
    return resultArr;

}

vector<Uint8> RLEDekompresja(vector<Uint8> input){
    int j = 0;
    //vector<Uint8> input = readVector<Uint8>(fileName);
    int length = input.size();
    vector<Uint8> output;
    while (j < length){
        //// cout <<"W" << input[j] << " ";
        if(input[j] > 0){
            int iter = input[j];
            // // cout <<"I" << iter << " ";
            j++;
            for(int i = 0; i < iter; i++){
                // cout << input[j] << ", ";
                output.push_back(input[j]);
            }
        } else if ( input[j] == 0){
            j++;
            int iter = input[j];
            for(int i = 0; i < iter; i++){
                j++;
                // cout << input[j] << ", ";
                output.push_back(input[j]);
            }

            if(iter % 2 != 0){
                j++;
            }
        }
        j++;
    }

    return output;
}

vector<token8> LZ77Kompresja(vector<Uint8> input, int length) {
    const int windowSize = 64000; // Rozmiar okna wyszukiwania
    int lookaheadBufferSize;
    vector<token8> resultArr;
    int position = 0;
    int numOfMatches = 0;
    while (position < length) {
        lookaheadBufferSize = windowSize - position;
        int matchDistance = 0;
        int matchLength = 0;
        int searchStart = 0;
        int searchEnd = position;


        // Wyszukiwanie najdłuższego dopasowania w oknie wyszukiwania
        for (int i = searchStart; i < searchEnd; i++) {
            int currentMatchLength = 0;
            while (currentMatchLength < lookaheadBufferSize &&
                   position + currentMatchLength < length &&
                   input[i + currentMatchLength] == input[position + currentMatchLength]) {
                currentMatchLength++;
            }

            if (currentMatchLength > matchLength) {
                matchLength = currentMatchLength;
                matchDistance = position - i;
            }
        }

        // Jeśli nie znaleziono dopasowania, zapisujemy literę bezpośrednio
        if (matchLength < 1) {
            resultArr.push_back(token8((Uint16)0, (Uint16)0, (Uint8)input[position]));
            position++;
            //cout << "RAW: ( " << (int)input[position] << " )" << endl;

        }
            // W przeciwnym razie zapisujemy parę (odległość, długość, następny znak)
        else {
            Uint8 nextChar = (position + matchLength < length) ? input[position + matchLength] : 0;
            resultArr.push_back(token8((Uint16)matchLength, (Uint16)matchDistance, (Uint8)nextChar));
            position += matchLength + 1;
            //cout << "( Len: " << (int)matchLength << ", Dist: " << (int)matchDistance << ", next: " << (int)nextChar << " )   Position: " << position << endl;
            numOfMatches++;
        }
    }
    return resultArr;
}

// Funkcja LZ77 - dekompresuje wejściowy wektor danych
vector<Uint8> LZ77Dekompresja(vector<token8> tokens) {
    //vector<token> tokens = readVector<token>(filename);
    vector<Uint8> output;
    for (token8 tok : tokens) {
        // Jeśli tokLength to 0, oznacza to, że zapisujemy surową wartość rawValue
        if (tok.tokLength == 0) {
            output.push_back(tok.rawValue);
        } else {
            // Zapisujemy odwołanie do wcześniejszych danych w strumieniu
            int startPos = output.size() - tok.shift;
            for (int i = 0; i < tok.tokLength; ++i) {
                output.push_back(output[startPos + i]);
            }

            // Zapisujemy dodatkowy znak rawValue
            output.push_back(tok.rawValue);
        }
    }

    return output;
}

vector<token16> LZ77Kompresja(vector<Uint16> input, int length) {
    const int windowSize = 64000; // Rozmiar okna wyszukiwania
    int lookaheadBufferSize;
    vector<token16> resultArr;
    int position = 0;
    int numOfMatches = 0;
    while (position < length) {
        lookaheadBufferSize = windowSize - position;
        int matchDistance = 0;
        int matchLength = 0;
        int searchStart = 0;
        int searchEnd = position;


        // Wyszukiwanie najdłuższego dopasowania w oknie wyszukiwania
        for (int i = searchStart; i < searchEnd; i++) {
            int currentMatchLength = 0;
            while (currentMatchLength < lookaheadBufferSize &&
                   position + currentMatchLength < length &&
                   input[i + currentMatchLength] == input[position + currentMatchLength]) {
                currentMatchLength++;
            }

            if (currentMatchLength > matchLength) {
                matchLength = currentMatchLength;
                matchDistance = position - i;
            }
        }

        // Jeśli nie znaleziono dopasowania, zapisujemy literę bezpośrednio
        if (matchLength < 1) {
            resultArr.push_back(token16((Uint16)0, (Uint16)0, (Uint16)input[position]));
            position++;
            //cout << "RAW: ( " << (int)input[position] << " )" << endl;

        }
            // W przeciwnym razie zapisujemy parę (odległość, długość, następny znak)
        else {
            Uint16 nextChar = (position + matchLength < length) ? input[position + matchLength] : 0;
            resultArr.push_back(token16((Uint16)matchLength, (Uint16)matchDistance, (Uint16)nextChar));
            position += matchLength + 1;
            //cout << "( Len: " << (int)matchLength << ", Dist: " << (int)matchDistance << ", next: " << (int)nextChar << " )   Position: " << position << endl;
            numOfMatches++;
        }
    }
    return resultArr;
}

// Funkcja LZ77 - dekompresuje wejściowy wektor danych
vector<Uint16> LZ77Dekompresja(vector<token16> tokens) {
    //vector<token> tokens = readVector<token>(filename);
    vector<Uint16> output;
    int j = 0;
    for (token16 tok : tokens) {
        // Jeśli tokLength to 0, oznacza to, że zapisujemy surową wartość rawValue
        if (tok.tokLength == 0) {
            output.push_back(tok.rawValue);
        } else {
            // Zapisujemy odwołanie do wcześniejszych danych w strumieniu
            int startPos = output.size() - tok.shift;
            for (int i = 0; i < tok.tokLength; ++i) {
                output.push_back(output[startPos + i]);
            }

            // Zapisujemy dodatkowy znak rawValue
            output.push_back(tok.rawValue);
        }
    }

    return output;
}


void wyswietlDane(macierz blok) {
    cout << "Dane w macierzy: " << endl;
    for(int y = 0; y < rozmiarBloku; y++) {
        for(int x = 0; x < rozmiarBloku; x++){
            cout << (int)blok.dane[x][y] << "   ";
        }
        cout << endl;
    }
}
void wyswietlDCT(macierz blok) {
    cout << "Macierz DCT: " << endl;
    for(int y = 0; y < rozmiarBloku; y++) {
        for(int x = 0; x < rozmiarBloku; x++){
            cout << blok.dct[x][y] << "   ";
        }
        cout << endl;
    }
}

macierz dct(Uint8 wartosci[rozmiarBloku][rozmiarBloku]) {
    float wynik[rozmiarBloku][rozmiarBloku];

    for(int v = 0; v < rozmiarBloku; ++v) {
        for(int u = 0; u < rozmiarBloku; ++u) {
            const double cu = (u == 0) ? 1.0 / sqrt(2) : 1.0;
            const double cv = (v == 0) ? 1.0 / sqrt(2) : 1.0;
            double wspolczynnikDCT = 0;

            for(int y = 0; y < rozmiarBloku; ++y) {
                for(int x = 0; x < rozmiarBloku; ++x) {
                    double uCosFactor = cos((double)(2*x+1)* M_PI * (double)u / (2 * (double)rozmiarBloku));
                    double vCosFactor = cos((double)(2*y+1)* M_PI * (double)v / (2 * (double)rozmiarBloku));
                    double pixel = (double)wartosci[x][y];
                    wspolczynnikDCT += pixel * uCosFactor *vCosFactor;
                }
            }
            wspolczynnikDCT *= (2.0 / (double)rozmiarBloku) * cu * cv;
            wynik[u][v] = wspolczynnikDCT;
        }
    }

    macierz rezultat;
    for(int j = 0; j < rozmiarBloku; j++) {
        for(int i = 0; i < rozmiarBloku; i++) {
            rezultat.dct[i][j] = wynik[i][j];
            rezultat.dane[i][j] = wartosci[i][j];
        }
    }
    return rezultat;
}

macierz idct(float DCT[rozmiarBloku][rozmiarBloku]) {
    int wynik[rozmiarBloku][rozmiarBloku];

    for(int x = 0; x < rozmiarBloku; ++x) {
        for (int y = 0; y < rozmiarBloku; ++y) {
            double pixel = 0;

            for(int u = 0; u < rozmiarBloku; ++u) {
                for(int v = 0; v < rozmiarBloku; ++v) {
                    const double cu = (u == 0) ? 1.0 / sqrt(2) : 1.0;
                    const double cv = (v == 0) ? 1.0 / sqrt(2) : 1.0;
                    double uCosFactor = cos((double)(2*x+1)* M_PI * (double)u / (2 * (double)rozmiarBloku));
                    double vCosFactor = cos((double)(2*y+1)* M_PI * (double)v / (2 * (double)rozmiarBloku));
                    double wspolczynnikDCT = DCT[u][v];
                    pixel += wspolczynnikDCT * uCosFactor * cu * vCosFactor * cv;
                }
            }
            pixel *= (2.0 / (double)rozmiarBloku);
            wynik[x][y] = round(pixel);
        }
    }

    macierz rezultat;
    for(int j = 0; j < rozmiarBloku; j++) {
        for(int i = 0; i < rozmiarBloku; i++) {
            if(wynik[i][j] > 255) wynik[i][j]=255;
            if(wynik[i][j] < 0) wynik[i][j]=0;
            rezultat.dane[i][j] = wynik[i][j];
            rezultat.dct[i][j] = DCT[i][j];
        }
    }
    return rezultat;
}

float findMaxABS(macierz blok, float lastMaxABS) {
    float max = 0;
    float min = 0;
    float data;
    for(int j = 0; j < rozmiarBloku; j++) {
        for (int i = 0; i < rozmiarBloku; i++) {
            data = blok.dct[i][j];
            if(data > max) {
                max = data;
            }
            if(data < min) {
                min = data;
            }
        }
    }
    if((min * -1) > max) {
        if((min * -1) > lastMaxABS) {
            return min * -1;
        } else {
            return lastMaxABS;
        }
    } else {
        if(max > lastMaxABS) {
            return max;
        } else {
            return lastMaxABS;
        }
    }
}

DCToutput DCTKompresja(Uint8 tryb) {
    macierz wejscie;
    macierz blokDCT;
    macierz wyjscie;
    float maxABS;
    float mnoznik;
    vector<Uint8> tablicaDoKompresji;
    vector<Uint8> tablicaPoKompresji;
    vector<macierz> allBlocks;
    vector<float> firstElements;

    int c = 0;
    int compressedSize = 0;

    if(tryb & 0x80) /* Color */ {
        if(tryb & 0x08) /* HSL */ {
        // po wysokosci -> 25 przebiegow
            for(int k = 0; k < (wysokosc / 2 / rozmiarBloku); k++) {
                // po szerokosci -> 40 przebiegow
                for(int l = 0; l < (szerokosc / 2 / rozmiarBloku); l++) {
                    // pobranie danych w bloku
                    for(int x = 0; x < rozmiarBloku; x++) {
                        for(int y = 0; y < rozmiarBloku; y++) {
                            wejscie.dane[x][y] = RGBtoHSL(x + (l * rozmiarBloku), y + (k * rozmiarBloku)).H;
                        }
                    }

                    // transformata
                    blokDCT = dct(wejscie.dane);
                    firstElements.push_back(blokDCT.dct[0][0]);
                    allBlocks.push_back(blokDCT);
                    maxABS = findMaxABS(blokDCT, maxABS);
                }
            }
            // po wysokosci -> 25 przebiegow
            for(int k = 0; k < (wysokosc / 2 / rozmiarBloku); k++) {
                // po szerokosci -> 40 przebiegow
                for(int l = 0; l < (szerokosc / 2 / rozmiarBloku); l++) {
                    // pobranie danych w bloku
                    for(int x = 0; x < rozmiarBloku; x++) {
                        for(int y = 0; y < rozmiarBloku; y++) {
                            wejscie.dane[x][y] = RGBtoHSL(x + (l * rozmiarBloku), y + (k * rozmiarBloku)).S;
                        }
                    }

                    // transformata
                    blokDCT = dct(wejscie.dane);
                    firstElements.push_back(blokDCT.dct[0][0]);
                    allBlocks.push_back(blokDCT);
                    maxABS = findMaxABS(blokDCT, maxABS);
                }
            }
            // po wysokosci -> 25 przebiegow
            for(int k = 0; k < (wysokosc / 2 / rozmiarBloku); k++) {
                // po szerokosci -> 40 przebiegow
                for(int l = 0; l < (szerokosc / 2 / rozmiarBloku); l++) {
                    // pobranie danych w bloku
                    for(int x = 0; x < rozmiarBloku; x++) {
                        for(int y = 0; y < rozmiarBloku; y++) {
                            wejscie.dane[x][y] = RGBtoHSL(x + (l * rozmiarBloku), y + (k * rozmiarBloku)).L;
                        }
                    }

                    // transformata
                    blokDCT = dct(wejscie.dane);
                    firstElements.push_back(blokDCT.dct[0][0]);
                    allBlocks.push_back(blokDCT);
                    maxABS = findMaxABS(blokDCT, maxABS);
                }
            }
        } else /* RGB */ {
            // po wysokosci -> 25 przebiegow
            for(int k = 0; k < (wysokosc / 2 / rozmiarBloku); k++) {
                // po szerokosci -> 40 przebiegow
                for(int l = 0; l < (szerokosc / 2 / rozmiarBloku); l++) {
                    // pobranie danych w bloku
                    for(int x = 0; x < rozmiarBloku; x++) {
                        for(int y = 0; y < rozmiarBloku; y++) {
                            wejscie.dane[x][y] = getPixel(x + (l * rozmiarBloku), y + (k * rozmiarBloku)).r;
                        }
                    }

                    // transformata
                    blokDCT = dct(wejscie.dane);
                    firstElements.push_back(blokDCT.dct[0][0]);
                    allBlocks.push_back(blokDCT);
                    maxABS = findMaxABS(blokDCT, maxABS);
                }
            }
            // po wysokosci -> 25 przebiegow
            for(int k = 0; k < (wysokosc / 2 / rozmiarBloku); k++) {
                // po szerokosci -> 40 przebiegow
                for(int l = 0; l < (szerokosc / 2 / rozmiarBloku); l++) {
                    // pobranie danych w bloku
                    for(int x = 0; x < rozmiarBloku; x++) {
                        for(int y = 0; y < rozmiarBloku; y++) {
                            wejscie.dane[x][y] = getPixel(x + (l * rozmiarBloku), y + (k * rozmiarBloku)).g;
                        }
                    }

                    // transformata
                    blokDCT = dct(wejscie.dane);
                    firstElements.push_back(blokDCT.dct[0][0]);
                    allBlocks.push_back(blokDCT);
                    maxABS = findMaxABS(blokDCT, maxABS);
                }
            }
            // po wysokosci -> 25 przebiegow
            for(int k = 0; k < (wysokosc / 2 / rozmiarBloku); k++) {
                // po szerokosci -> 40 przebiegow
                for(int l = 0; l < (szerokosc / 2 / rozmiarBloku); l++) {
                    // pobranie danych w bloku
                    for(int x = 0; x < rozmiarBloku; x++) {
                        for(int y = 0; y < rozmiarBloku; y++) {
                            wejscie.dane[x][y] = getPixel(x + (l * rozmiarBloku), y + (k * rozmiarBloku)).b;
                        }
                    }

                    // transformata
                    blokDCT = dct(wejscie.dane);
                    firstElements.push_back(blokDCT.dct[0][0]);
                    allBlocks.push_back(blokDCT);
                    maxABS = findMaxABS(blokDCT, maxABS);
                }
            }
        }
    } else /* BW */ {
        if(tryb & 0x08) /* HSL */ {
            // po wysokosci -> 25 przebiegow
            for (int k = 0; k < (wysokosc / 2 / rozmiarBloku); k++) {
                // po szerokosci -> 40 przebiegow
                for (int l = 0; l < (szerokosc / 2 / rozmiarBloku); l++) {
                    // pobranie danych w bloku
                    for (int x = 0; x < rozmiarBloku; x++) {
                        for (int y = 0; y < rozmiarBloku; y++) {
                            wejscie.dane[x][y] = RGBtoHSL(x + (l * rozmiarBloku), y + (k * rozmiarBloku)).L;
                        }
                    }
                    // transformata
                    blokDCT = dct(wejscie.dane);
                    firstElements.push_back(blokDCT.dct[0][0]);
                    allBlocks.push_back(blokDCT);
                    maxABS = findMaxABS(blokDCT, maxABS);
                }
            }
        } else {
            // po wysokosci -> 25 przebiegow
            for (int k = 0; k < (wysokosc / 2 / rozmiarBloku); k++) {
                // po szerokosci -> 40 przebiegow
                for (int l = 0; l < (szerokosc / 2 / rozmiarBloku); l++) {
                    // pobranie danych w bloku
                    for (int x = 0; x < rozmiarBloku; x++) {
                        for (int y = 0; y < rozmiarBloku; y++) {
                            wejscie.dane[x][y] = getPixel(x + (l * rozmiarBloku), y + (k * rozmiarBloku)).r;
                        }
                    }
                    // transformata
                    blokDCT = dct(wejscie.dane);
                    firstElements.push_back(blokDCT.dct[0][0]);
                    allBlocks.push_back(blokDCT);
                    maxABS = findMaxABS(blokDCT, maxABS);
                }
            }
        }
    }

    mnoznik = maxABS / 128.0;

    // kwantyzacja i normalizacja wspolczynnikow
    for(int i = 0; i < allBlocks.size(); i++) {
        for(int x = 0; x < rozmiarBloku; x++) {
            for(int y = 0; y < rozmiarBloku; y++) {
                if(x == 0 && y == 0) {
                    continue;
                }
                allBlocks[i].dct[x][y] /= mnoznik;
                allBlocks[i].dct[x][y] += 127.0;
                allBlocks[i].dct[x][y] = round(allBlocks[i].dct[x][y]);
            }
        }
    }


    // zapis do tablicy do kompresji
    for(int i = 0; i < allBlocks.size(); i++) {
        int x = 1, y = 0;  // Start from (1,0) since (0,0) should be skipped
        bool goingUp = true;

        while (x >= 0 && x < 8 && y >= 0 && y < 8) {
            // Process current element
            tablicaDoKompresji.push_back((Uint8) allBlocks[i].dct[x][y]);

            if (goingUp) {
                if (x == 0 || y == 7) {  // Hit top or right boundary
                    if (y == 7) {  // Hit right boundary
                        x++;
                    } else {  // Hit top boundary
                        y++;
                    }
                    goingUp = false;  // Change direction
                } else {
                    x--;  // Move diagonally up-right
                    y++;
                }
            } else {  // Going down
                if (x == 7 || y == 0) {  // Hit bottom or left boundary
                    if (x == 7) {  // Hit bottom boundary
                        y++;
                    } else {  // Hit left boundary
                        x++;
                    }
                    goingUp = true;  // Change direction
                } else {
                    x++;  // Move diagonally down-left
                    y--;
                }
            }

            // Break if we go out of bounds
            if (x >= 8 || y >= 8) {
                break;
            }
        }
    }

    tablicaPoKompresji = RLEKompresja(tablicaDoKompresji, tablicaDoKompresji.size());
    allBlocks.clear();
    tablicaDoKompresji.clear();
    DCToutput output;
    output.mnoznik = mnoznik;
    output.pierwszeWspolczynniki = firstElements;
    output.reszta = tablicaPoKompresji;
    tablicaPoKompresji.clear();
    firstElements.clear();
    return output;
}

vector<macierz> DCTDekompresja(DCToutput input, Uint8 tryb) {

    vector<Uint8> tablicaPoDekompresji;
    vector<macierz> allBlocks;
    vector<macierz> outputAllBlocks;

    // dekompresja
    tablicaPoDekompresji = RLEDekompresja(input.reszta);
    if(tryb & 0x80) /* Color */ {
        int vectorIndex = 0;
        for(int i = 0; i < 3000; i++) {
            macierz m;
            int x = 1, y = 0;  // Start from (1,0) since (0,0) should be skipped
            bool goingUp = true;

            while (x >= 0 && x < 8 && y >= 0 && y < 8) {
                // Process current element
                m.dct[x][y] = tablicaPoDekompresji[vectorIndex];
                vectorIndex++;

                if (goingUp) {
                    if (x == 0 || y == 7) {  // Hit top or right boundary
                        if (y == 7) {  // Hit right boundary
                            x++;
                        } else {  // Hit top boundary
                            y++;
                        }
                        goingUp = false;  // Change direction
                    } else {
                        x--;  // Move diagonally up-right
                        y++;
                    }
                } else {  // Going down
                    if (x == 7 || y == 0) {  // Hit bottom or left boundary
                        if (x == 7) {  // Hit bottom boundary
                            y++;
                        } else {  // Hit left boundary
                            x++;
                        }
                        goingUp = true;  // Change direction
                    } else {
                        x++;  // Move diagonally down-left
                        y--;
                    }
                }

                // Break if we go out of bounds
                if (x >= 8 || y >= 8) {
                    break;
                }
            }
            allBlocks.push_back(m);
        }
    } else /* No color */ {
        macierz m;
        int vectorIndex = 0;
        for(int i = 0; i < 1000; i++) {
            int x = 1, y = 0;  // Start from (1,0) since (0,0) should be skipped
            bool goingUp = true;

            while (x >= 0 && x < 8 && y >= 0 && y < 8) {
                // Process current element
                m.dct[x][y] = tablicaPoDekompresji[vectorIndex];
                vectorIndex++;

                if (goingUp) {
                    if (x == 0 || y == 7) {  // Hit top or right boundary
                        if (y == 7) {  // Hit right boundary
                            x++;
                        } else {  // Hit top boundary
                            y++;
                        }
                        goingUp = false;  // Change direction
                    } else {
                        x--;  // Move diagonally up-right
                        y++;
                    }
                } else {  // Going down
                    if (x == 7 || y == 0) {  // Hit bottom or left boundary
                        if (x == 7) {  // Hit bottom boundary
                            y++;
                        } else {  // Hit left boundary
                            x++;
                        }
                        goingUp = true;  // Change direction
                    } else {
                        x++;  // Move diagonally down-left
                        y--;
                    }
                }

                // Break if we go out of bounds
                if (x >= 8 || y >= 8) {
                    break;
                }
            }
            allBlocks.push_back(m);
        }
    }

    tablicaPoDekompresji.clear();

    // odwrócenie kwantyzacji i normalizacji
    for(int i = 0; i < allBlocks.size(); i++) {
        for(int x = 0; x < 8; x++) {
            for(int y = 0; y < 8; y++) {
                allBlocks[i].dct[x][y] -= 127.0;
                allBlocks[i].dct[x][y] *= input.mnoznik;
            }
        }
        allBlocks[i].dct[0][0] = input.pierwszeWspolczynniki[i];
    }


    for(int i = 0; i < allBlocks.size(); i++) {
        outputAllBlocks.push_back(idct(allBlocks[i].dct));
    }

    allBlocks.clear();

    return outputAllBlocks;
}


