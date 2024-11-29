// funkcje do operacji na plikach
#include "Funkcje.h"
#include "Zmienne.h"
#include "Paleta.h"
#include "MedianCut.h"
#include "Pliki.h"
#include "Modele.h"

Uint8 shiftByte(Uint8 b, Uint8 shiftAmount) {
    b = b << 8 - shiftAmount;
    return b;
}

void ZapiszPlik(string a){
    Uint8 kolor1, extractedBits;
    int counter = 0;

    cout<<"\nZapisujemy plik '"<< a<< ".z21' uzywajac metody write()"<<endl;
    ofstream wyjscie((a + ".z21").c_str() , ios::binary);

    wyjscie.write((char*)&identyfikator, sizeof(char)*4);
    wyjscie.write((char*)&szerokoscObrazka, sizeof(Uint16));
    wyjscie.write((char*)&wysokoscObrazka, sizeof(Uint16));
    wyjscie.write((char*)&tryb, sizeof(Uint8));
    wyjscie.write((char*)&dithering, sizeof(Uint8));
   if(tryb == 3 || tryb == 5){
        rozmiarPalety = ileKubelkow;
        wyjscie.write((char*)&rozmiarPalety, sizeof(Uint8));
        for(int i = 0; i < rozmiarPalety; i++){
            wyjscie.write((char*)&paleta7[i].r, sizeof(Uint8));
            wyjscie.write((char*)&paleta7[i].g, sizeof(Uint8));
            wyjscie.write((char*)&paleta7[i].b, sizeof(Uint8));
        }
    }

    SDL_Color kolor;
    memset(paleta7DoPlikuUint8,0,sizeof(paleta7DoPlikuUint8));

    if(tryb == 1 || tryb == 4 || tryb == 5){  //KOLOR
        int kolor7bit;
        for(int z = szerokosc/2; z<szerokosc; z += 8){
            for(int y = 0; y < wysokosc/2; y++){
                for(int x = z; x < z+8; x++){
                    kolor = getPixel(x,y);

                    kolor7bit = z24RGBna7RGB(kolor);
                    paleta7DoPlikuUint8[counter] = kolor7bit;
                    counter++;
                }
            }
        }

    }else if(tryb == 2 || tryb == 3){ //BW
        int szary7bit;
        for(int z = szerokosc/2; z<szerokosc; z += 8){
            for(int y = 0; y < wysokosc/2; y++){
                for(int x = z; x < z+8; x++){
                    kolor = getPixel(x,y);

                    szary7bit = z24RGBna7BW(kolor);
                    paleta7DoPlikuUint8[counter] = szary7bit;
                    counter++;
                }
            }
        }
    }

    if(tryb != 0){
        for(int x = 1; x < sizeof(paleta7DoPlikuUint8); x += 8){
            for(int i = 1; i <= 7; i++){
                extractedBits = (paleta7DoPlikuUint8[x+i-1] >> (8 - i)) & ((1 << i) - 1);   // ekstrakturemy 'i' ostatnich bit�w z aktualnego elementu
                paleta7DoPlikuUint8[x+i-1] = paleta7DoPlikuUint8[x+i-1] << i;               // przesuwamy aktualn� warto�� tabeli o 'i' bit�w
                paleta7DoPlikuUint8[x+i-2] = paleta7DoPlikuUint8[x+i-2] | extractedBits;    // bramk� OR ��czymy wyekstrakowane bity z poprzedni� warto�ci� w tabeli

                wyjscie.write((char*)&paleta7DoPlikuUint8[x+i-2], sizeof(Uint8));
            }
        }
        cout << "Gotowe!" << endl;
    }else{
        cout << "Nie wybrano trybu" << endl;
    }

    wyjscie.close();
    memset(paleta7DoPlikuUint8,0,sizeof(paleta7DoPlikuUint8));
    memset(paleta7,0,sizeof(paleta7));

}


void OdczytajPlik(string a){
    czyscEkran(0,0,0);
    SDL_Color kolor;
    Uint8 kolorUint8;
    int counter = 0;
    char identyfikator[] = "    ";
    Uint16 szerokoscObrazka = 0;
    Uint16 wysokoscObrazka = 0;
    Uint8 tryb = 0;
    Uint8 dithering = 0;
    Uint8 rozmiarPalety = 0;
    Uint8 bitShiftedValue;
    Uint8 extractedBits, extractedBits2;
    Uint8 extractedBitsShifted;
    int offset = 0;

    cout<<"\nOdczytujemy plik '"<< a <<".z21' uzywajac metody read()"<<endl;
    ifstream wejscie((a + ".z21").c_str() ,ios::binary);
    wejscie.read((char*)&identyfikator,sizeof(char)*4);
    wejscie.read((char*)&szerokoscObrazka,sizeof(Uint16));
    wejscie.read((char*)&wysokoscObrazka,sizeof(Uint16));
    wejscie.read((char*)&tryb,sizeof(Uint8));
    wejscie.read((char*)&dithering,sizeof(Uint8));

    Uint8 paleta7ZPlikuUint7[szerokoscObrazka*wysokoscObrazka];
    Uint8 paleta7ZPlikuUint8[szerokoscObrazka*wysokoscObrazka];
    memset(paleta7ZPlikuUint7,0,sizeof(paleta7ZPlikuUint7));
    memset(paleta,0,sizeof(paleta));

    if(tryb == 3 || tryb == 5){
        wejscie.read((char*)&rozmiarPalety, sizeof(Uint8));
        for(int i = 0; i < rozmiarPalety; i++){
            wejscie.read((char*)&paleta[i].r, sizeof(Uint8));
            wejscie.read((char*)&paleta[i].g, sizeof(Uint8));
            wejscie.read((char*)&paleta[i].b, sizeof(Uint8));
        }
        narysujPalete(paleta);
    }

    if(tryb != 0){
        while(!wejscie.eof()){
            wejscie.read((char*)&paleta7ZPlikuUint7[counter],sizeof(Uint8));
            counter++;
        }
        counter = 0;
        for(int x = 1; x < sizeof(paleta7ZPlikuUint7); x += 7){
                for(int i = 1; i <= 6; i++){
                    if(i == 1){
                        paleta7ZPlikuUint8[x+i-2+offset] = paleta7ZPlikuUint7[x+i-2] >> 1;
                        paleta7ZPlikuUint8[x+i-2+offset] = paleta7ZPlikuUint8[x+i-2+offset] << 1;
                    }
                    extractedBits = paleta7ZPlikuUint7[x+i-2] & ((1 << i) - 1); // ekstraktujemy 'i' ostanich bit�w poprzedniej warto�ci
                    extractedBits = shiftByte(extractedBits, i);                          // odwracamy zekstrakowane bity

                    paleta7ZPlikuUint8[x+i-1+offset] = paleta7ZPlikuUint7[x+i-1] >> i+1;
                    paleta7ZPlikuUint8[x+i-1+offset] = paleta7ZPlikuUint8[x+i-1+offset] << 1;
                    paleta7ZPlikuUint8[x+i-1+offset] = paleta7ZPlikuUint8[x+i-1+offset] | extractedBits;

                    if(i == 6){
                        extractedBits = paleta7ZPlikuUint7[x+i-1] & ((1 << i+1) - 1);
                        extractedBits = shiftByte(extractedBits, i+1);
                        paleta7ZPlikuUint8[x+i+offset] = 0b00000000 | extractedBits;
                    }
                }
                offset++;
        }
        counter = 0;
        if(tryb == 1 || tryb == 5 || tryb == 4){
            for(int z = 0; z<szerokosc/2; z += 8){
                for(int y = 0; y < wysokosc/2; y++){
                    for(int x = z; x < z+8; x++){
                        kolorUint8 = paleta7ZPlikuUint8[counter];
                        counter++;
                        kolor = z7RGBna24RGB(kolorUint8);

                        setPixel(x,y,kolor.r,kolor.g,kolor.b);
                    }
                }
            }
        }else if(tryb == 2 || tryb == 3){
            for(int z = 0; z<szerokosc/2; z += 8){
                for(int y = 0; y < wysokosc/2; y++){
                    for(int x = z; x < z+8; x++){
                        kolorUint8 = paleta7ZPlikuUint8[counter];
                        counter++;
                        kolor = z7BWna24RGB(kolorUint8);

                        setPixel(x,y,kolor.r,kolor.g,kolor.b);

                    }
                }
            }
        }else{
            cout << "Nieprawidlowy tryb" << endl;
        }
    }

    cout << "Odczyt z pliku\n";
    cout << "Id: " << identyfikator << endl;
    cout << "szerokosc: " << szerokoscObrazka << endl;
    cout << "wysokosc: " << wysokoscObrazka << endl;
    cout << "tryb: " << (int)tryb << endl;
    cout << "dithering: " << (int)dithering << endl;
    cout << "rozmiar palety:" << (int)rozmiarPalety << endl << endl;

 SDL_UpdateWindowSurface(window);
}

struct tabelaRGB{
    Uint8 r[320][200];
    Uint8 g[320][200];
    Uint8 b[320][200];
};

void ZapiszModel(string a, int tryb){

    SDL_Color color;
    YUV yuv;
    YIQ yiq;
    YCbCr ycbcr;
    HSL hsl;
    tabelaRGB tabela;

    cout<<"\nZapisujemy plik '"<< a<< ".z21' uzywajac metody write()"<<endl;
    ofstream wyjscie((a + ".z21").c_str() , ios::binary);

    wyjscie.write((char*)&identyfikator, sizeof(char)*4);
    wyjscie.write((char*)&szerokoscObrazka, sizeof(Uint16));
    wyjscie.write((char*)&wysokoscObrazka, sizeof(Uint16));
    wyjscie.write((char*)&tryb, sizeof(Uint8));

    // RGB888
    if (tryb == 10) {
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                color = getPixel(x,y);
                tabela.r[x][y] = color.r;
                tabela.g[x][y] = color.g;
                tabela.b[x][y] = color.b;
                wyjscie.write((char*)&tabela.r[x][y], sizeof(Uint8));
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wyjscie.write((char*)&tabela.g[x][y], sizeof(Uint8));
            }
        }

        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wyjscie.write((char*)&tabela.b[x][y], sizeof(Uint8));
            }
        }
    }
    // YUV888
    else if (tryb == 11) {
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                yuv = RGBtoYUV(x,y);
                tabela.r[x][y] = yuv.Y;
                tabela.g[x][y] = yuv.U;
                tabela.b[x][y] = yuv.V;
                wyjscie.write((char*)&tabela.r[x][y], sizeof(Uint8));
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wyjscie.write((char*)&tabela.g[x][y], sizeof(Uint8));
            }
        }

        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wyjscie.write((char*)&tabela.b[x][y], sizeof(Uint8));
            }
        }
    }
    // YIQ888
    else if (tryb == 12) {
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                yiq = RGBtoYIQ(x,y);
                tabela.r[x][y] = yiq.Y;
                tabela.g[x][y] = yiq.I;
                tabela.b[x][y] = yiq.Q;
                wyjscie.write((char*)&tabela.r[x][y], sizeof(Uint8));
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wyjscie.write((char*)&tabela.g[x][y], sizeof(Uint8));
            }
        }

        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wyjscie.write((char*)&tabela.b[x][y], sizeof(Uint8));
            }
        }
    }
    // YCbCr888
    else if (tryb == 13) {
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                ycbcr = RGBtoYCbCr(x,y);
                tabela.r[x][y] = ycbcr.Y;
                tabela.g[x][y] = ycbcr.Cb;
                tabela.b[x][y] = ycbcr.Cr;
                wyjscie.write((char*)&tabela.r[x][y], sizeof(Uint8));
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wyjscie.write((char*)&tabela.g[x][y], sizeof(Uint8));
            }
        }

        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wyjscie.write((char*)&tabela.b[x][y], sizeof(Uint8));
            }
        }
    }
    // HSL888
    else if (tryb == 14) {
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                hsl = RGBtoHSL(x,y);
                tabela.r[x][y] = hsl.H;
                tabela.g[x][y] = hsl.S;
                tabela.b[x][y] = hsl.L;
                wyjscie.write((char*)&tabela.r[x][y], sizeof(Uint8));
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wyjscie.write((char*)&tabela.g[x][y], sizeof(Uint8));
            }
        }

        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wyjscie.write((char*)&tabela.b[x][y], sizeof(Uint8));
            }
        }
    }
    // RGB555
    else if (tryb == 15) {
        Uint16 rgb555;
        for(int x =0; x<szerokosc/2; x++) {
            for (int y = 0; y < wysokosc / 2; y++) {
                rgb555 = getRGB555(x, y);
                wyjscie.write((char*)&rgb555, sizeof(Uint16));
            }
        }
    }
    // RGB565
    else if (tryb == 16) {
        Uint16 rgb565;
        for(int x =0; x<szerokosc/2; x++) {
            for (int y = 0; y < wysokosc / 2; y++) {
                rgb565 = getRGB565(x, y);
                wyjscie.write((char*)&rgb565, sizeof(Uint16));
            }
        }
    } else {
        cout << "Nieprawidłowy typ!" << endl;
    }

    wyjscie.close();
}

void OdczytajModel(string a){
    czyscEkran(0,0,0);
    SDL_Color kolor;
    YUV yuv;
    YIQ yiq;
    YCbCr ycbcr;
    HSL hsl;
    Uint8 kolorUint8;
    int counter = 0;
    char identyfikator[] = "    ";
    Uint16 szerokoscObrazka = 0;
    Uint16 wysokoscObrazka = 0;
    Uint8 tryb = 0;
    tabelaRGB tablica;

    cout<<"\nOdczytujemy plik '"<< a <<".z21' uzywajac metody read()"<<endl;
    ifstream wejscie((a + ".z21").c_str() ,ios::binary);
    wejscie.read((char*)&identyfikator,sizeof(char)*4);
    wejscie.read((char*)&szerokoscObrazka,sizeof(Uint16));
    wejscie.read((char*)&wysokoscObrazka,sizeof(Uint16));
    wejscie.read((char*)&tryb,sizeof(Uint8));

    // RGB888
    if(tryb == 10) {
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wejscie.read((char*)&kolor.r, sizeof(Uint8));
                tablica.r[x][y] = kolor.r;
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wejscie.read((char*)&kolor.g, sizeof(Uint8));
                tablica.g[x][y] = kolor.g;
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wejscie.read((char*)&kolor.b, sizeof(Uint8));
                tablica.b[x][y] = kolor.b;
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                setPixel(x,y, tablica.r[x][y], tablica.g[x][y], tablica.b[x][y]);

            }
        }
    }
    // YUV888
    else if(tryb == 11) {
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wejscie.read((char*)&yuv.Y, sizeof(Uint8));
                tablica.r[x][y] = yuv.Y;
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wejscie.read((char*)&yuv.U, sizeof(Uint8));
                tablica.g[x][y] = yuv.U;
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wejscie.read((char*)&yuv.V, sizeof(Uint8));
                tablica.b[x][y] = yuv.V;
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                kolor = YUVtoRGB(tablica.r[x][y], tablica.g[x][y], tablica.b[x][y]);
                setPixel(x,y, kolor.r, kolor.g, kolor.b);

            }
        }
    } else if(tryb == 12) {
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wejscie.read((char*)&yiq.Y, sizeof(Uint8));
                tablica.r[x][y] = yiq.Y;
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wejscie.read((char*)&yiq.I, sizeof(Uint8));
                tablica.g[x][y] = yiq.I;
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wejscie.read((char*)&yiq.Q, sizeof(Uint8));
                tablica.b[x][y] = yiq.Q;
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                kolor = YIQtoRGB(tablica.r[x][y], tablica.g[x][y], tablica.b[x][y]);
                setPixel(x,y, kolor.r, kolor.g, kolor.b);

            }
        }
    } else if(tryb == 13) {
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wejscie.read((char*)&ycbcr.Y, sizeof(Uint8));
                tablica.r[x][y] = ycbcr.Y;
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wejscie.read((char*)&ycbcr.Cb, sizeof(Uint8));
                tablica.g[x][y] = ycbcr.Cb;
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wejscie.read((char*)&ycbcr.Cr, sizeof(Uint8));
                tablica.b[x][y] = ycbcr.Cr;
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                kolor = YCbCrtoRGB(tablica.r[x][y], tablica.g[x][y], tablica.b[x][y]);
                setPixel(x,y, kolor.r, kolor.g, kolor.b);

            }
        }
    } else if(tryb == 14) {
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wejscie.read((char*)&hsl.H, sizeof(Uint8));
                tablica.r[x][y] = hsl.H;
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wejscie.read((char*)&hsl.S, sizeof(Uint8));
                tablica.g[x][y] = hsl.S;
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                wejscie.read((char*)&hsl.L, sizeof(Uint8));
                tablica.b[x][y] = hsl.L;
            }
        }
        for(int x =0; x<szerokosc/2; x++){
            for(int y = 0; y<wysokosc/2; y++){
                kolor = HSLtoRGB(tablica.r[x][y], tablica.g[x][y], tablica.b[x][y]);
                setPixel(x,y, kolor.r, kolor.g, kolor.b);

            }
        }
    } else if(tryb == 15) {
        Uint16 rgb555;
        for(int x =0; x<szerokosc/2; x++) {
            for (int y = 0; y < wysokosc / 2; y++) {
                wejscie.read((char*)&rgb555, sizeof(Uint16));
                setRGB555(x, y, rgb555);
            }
        }
    } else if(tryb == 16) {
        Uint16 rgb565;
        for(int x =0; x<szerokosc/2; x++) {
            for (int y = 0; y < wysokosc / 2; y++) {
                wejscie.read((char*)&rgb565, sizeof(Uint16));
                setRGB565(x, y, rgb565);
            }
        }
    } else {
        cout << "Nieprawidłowy typ!" << endl;
    }
    wejscie.close();


    cout << "Odczyt z pliku\n";
    cout << "Id: " << identyfikator << endl;
    cout << "szerokosc: " << szerokoscObrazka << endl;
    cout << "wysokosc: " << wysokoscObrazka << endl;
    SDL_UpdateWindowSurface(window);
}

