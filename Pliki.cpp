// funkcje do operacji na plikach
#include "Funkcje.h"
#include "Zmienne.h"
#include "Paleta.h"
#include "MedianCut.h"
#include "Pliki.h"
#include "Modele.h"
#include "Kompresja.h"
#include <vector>

Uint8 shiftByte(Uint8 b, Uint8 shiftAmount) {
    b = b << 8 - shiftAmount;
    return b;
}

void ZapiszPlik(string a, Uint8 tryb){
    Uint8 kolor1, extractedBits;
    int counter = 0;

    cout<<"\nZapisujemy plik '"<< a<< ".z21' uzywajac metody write()"<<endl;
    ofstream wyjscie((a + ".z21").c_str() , ios::binary);

    wyjscie.write((char*)&identyfikator, sizeof(char)*4);
    wyjscie.write((char*)&szerokoscObrazka, sizeof(Uint16));
    wyjscie.write((char*)&wysokoscObrazka, sizeof(Uint16));
    wyjscie.write((char*)&tryb, sizeof(Uint8));
    // jeśli DCT
    if(tryb & 0x02){
        DCToutput dct = dctOutput;
        wyjscie.write((char*)&dct.mnoznik, sizeof(float));
        cout << dct.pierwszeWspolczynniki.size() << endl;
        cout << dct.reszta.size() << endl;
        for(int i = 0; i < dct.pierwszeWspolczynniki.size(); i++) {
            wyjscie.write((char*)&dct.pierwszeWspolczynniki[i], sizeof(float));
        }
        for(int i = 0; i < dct.reszta.size(); i++) {
            wyjscie.write((char*)&dct.reszta[i], sizeof(Uint8));
        }
        dct.pierwszeWspolczynniki.clear();
        dct.reszta.clear();
    } else {
        // jeśli predykcja
        if(tryb & 0x10) {
            if(tryb & 0x40) {
                if(tryb & 0x08) {
                    if(tryb & 0x80) {
                        vector<Uint8> R;
                        vector<Uint8> G;
                        vector<Uint8> B;
                        vector<Uint8> Rp;
                        vector<Uint8> Gp;
                        vector<Uint8> Bp;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                HSL hsl = RGBtoHSL(x, y);
                                R.push_back(hsl.H);
                                G.push_back(hsl.S);
                                B.push_back(hsl.L);
                            }
                        }
                        Rp = filtrRoznicowy(R);
                        Gp = filtrRoznicowy(G);
                        Bp = filtrRoznicowy(B);
                        Rp.insert(Rp.end(), Gp.begin(), Gp.end());
                        Rp.insert(Rp.end(), Bp.begin(), Bp.end());
                        if(tryb & 0x01) {
                            vector<token8> lz77 = LZ77Kompresja(Rp, Rp.size());
                            for(int i = 0; i < lz77.size(); i++) {
                                wyjscie.write((char*)&lz77[i], sizeof(token8));
                            }
                            lz77.clear();
                        } else {
                            for(int i = 0; i < Rp.size(); i++) {
                                wyjscie.write((char*)&Rp[i], sizeof(Uint8));
                            }
                        }
                        R.clear();
                        G.clear();
                        B.clear();
                        Rp.clear();
                        Gp.clear();
                        Bp.clear();
                    } else {
                        vector<Uint8> L;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                HSL hsl = RGBtoHSL(x, y);
                                L.push_back(hsl.L);
                            }
                        }
                        vector<Uint8> Rp = filtrRoznicowy(L);
                        if(tryb & 0x01) {
                            vector<token8> lz77 = LZ77Kompresja(Rp, Rp.size());
                            for(int i = 0; i < lz77.size(); i++) {
                                wyjscie.write((char*)&lz77[i], sizeof(token8));
                            }
                            lz77.clear();
                        } else {
                            for(int i = 0; i < Rp.size(); i++) {
                                wyjscie.write((char*)&Rp[i], sizeof(Uint8));
                            }
                        }
                        Rp.clear();
                        L.clear();
                    }
                } else {
                    if(tryb & 0x80) {
                        vector<Uint8> R;
                        vector<Uint8> G;
                        vector<Uint8> B;
                        vector<Uint8> Rp;
                        vector<Uint8> Gp;
                        vector<Uint8> Bp;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                SDL_Color pixel = getPixel(x, y);
                                R.push_back(pixel.r);
                                G.push_back(pixel.g);
                                B.push_back(pixel.b);
                            }
                        }
                        Rp = filtrRoznicowy(R);
                        Gp = filtrRoznicowy(G);
                        Bp = filtrRoznicowy(B);
                        Rp.insert(Rp.end(), Gp.begin(), Gp.end());
                        Rp.insert(Rp.end(), Bp.begin(), Bp.end());
                        if(tryb & 0x01) {
                            vector<token8> lz77 = LZ77Kompresja(Rp, Rp.size());
                            for(int i = 0; i < lz77.size(); i++) {
                                wyjscie.write((char*)&lz77[i], sizeof(token8));
                            }
                            lz77.clear();
                        } else {
                            for(int i = 0; i < Rp.size(); i++) {
                                wyjscie.write((char*)&Rp[i], sizeof(Uint8));
                            }
                        }
                        R.clear();
                        G.clear();
                        B.clear();
                        Rp.clear();
                        Gp.clear();
                        Bp.clear();
                    } else {
                        vector<Uint8> R;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                SDL_Color pixel = getPixel(x, y);
                                R.push_back(pixel.r);
                            }
                        }
                        vector<Uint8> Rp = filtrRoznicowy(R);
                        if(tryb & 0x01) {
                            vector<token8> lz77 = LZ77Kompresja(Rp, Rp.size());
                            for(int i = 0; i < lz77.size(); i++) {
                                wyjscie.write((char*)&lz77[i], sizeof(token8));
                            }
                            lz77.clear();
                        } else {
                            for(int i = 0; i < Rp.size(); i++) {
                                wyjscie.write((char*)&Rp[i], sizeof(Uint8));
                            }
                        }
                        R.clear();
                        Rp.clear();
                    }
                }
            } else {
                if(tryb & 0x80) {
                    vector<Uint16> obraz;
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            obraz.push_back(getRGB565(x, y));
                        }
                    }
                    vector<Uint16> obrazP = filtrRoznicowy(obraz);
                    if(tryb & 0x01) {
                        vector<token16> lz77 = LZ77Kompresja(obrazP, obrazP.size());
                        for(int i = 0; i < lz77.size(); i++) {
                            wyjscie.write((char*)&lz77[i], sizeof(token16));
                        }
                        lz77.clear();
                    } else {
                        for(int i = 0; i < obrazP.size(); i++) {
                            wyjscie.write((char*)&obrazP[i], sizeof(Uint16));
                        }
                    }
                    obraz.clear();
                    obrazP.clear();
                } else {
                    vector<Uint8> obraz;
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            obraz.push_back(getRGB565_(x, y).g);
                        }
                    }
                    vector<Uint8> obrazP = filtrRoznicowy(obraz);
                    if(tryb & 0x01) {
                        vector<token8> lz77 = LZ77Kompresja(obrazP, obrazP.size());
                        for(int i = 0; i < lz77.size(); i++) {
                            wyjscie.write((char*)&lz77[i], sizeof(token8));
                        }
                        lz77.clear();
                    } else {
                        for(int i = 0; i < obrazP.size(); i++) {
                            wyjscie.write((char*)&obrazP[i], sizeof(Uint8));
                        }
                    }
                    obrazP.clear();
                    obraz.clear();
                }
            }
        } else /* BRAK PREDYKCJI */ {
            if(tryb & 0x40) {
                if(tryb & 0x08) {
                    if(tryb & 0x80) {
                        vector<Uint8> R;
                        vector<Uint8> G;
                        vector<Uint8> B;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                HSL hsl = RGBtoHSL(x, y);
                                R.push_back(hsl.H);
                                G.push_back(hsl.S);
                                B.push_back(hsl.L);
                            }
                        }
                        R.insert(R.end(), G.begin(), G.end());
                        R.insert(R.end(), B.begin(), B.end());
                        if(tryb & 0x01) {
                            vector<token8> lz77 = LZ77Kompresja(R, R.size());
                            for(int i = 0; i < lz77.size(); i++) {
                                wyjscie.write((char*)&lz77[i], sizeof(token8));
                            }
                            lz77.clear();
                        } else {
                            for(int i = 0; i < R.size(); i++) {
                                wyjscie.write((char*)&R[i], sizeof(Uint8));
                            }
                        }
                        R.clear();
                        G.clear();
                        B.clear();
                    } else {
                        vector<Uint8> L;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                HSL hsl = RGBtoHSL(x, y);
                                L.push_back(hsl.L);
                            }
                        }
                        if(tryb & 0x01) {
                            vector<token8> lz77 = LZ77Kompresja(L, L.size());
                            for(int i = 0; i < lz77.size(); i++) {
                                wyjscie.write((char*)&lz77[i], sizeof(token8));
                            }
                            lz77.clear();
                        } else {
                            for(int i = 0; i < L.size(); i++) {
                                wyjscie.write((char*)&L[i], sizeof(Uint8));
                            }
                        }
                        L.clear();
                    }
                } else {
                    if(tryb & 0x80) {
                        vector<Uint8> R;
                        vector<Uint8> G;
                        vector<Uint8> B;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                SDL_Color pixel = getPixel(x, y);
                                R.push_back(pixel.r);
                                G.push_back(pixel.g);
                                B.push_back(pixel.b);
                            }
                        }
                        R.insert(R.end(), G.begin(), G.end());
                        R.insert(R.end(), B.begin(), B.end());
                        if(tryb & 0x01) {
                            vector<token8> lz77 = LZ77Kompresja(R, R.size());
                            for(int i = 0; i < lz77.size(); i++) {
                                wyjscie.write((char*)&lz77[i], sizeof(token8));
                            }
                            lz77.clear();
                        } else {
                            for(int i = 0; i < R.size(); i++) {
                                wyjscie.write((char*)&R[i], sizeof(Uint8));
                            }
                        }
                        R.clear();
                        G.clear();
                        B.clear();
                    } else {
                        vector<Uint8> R;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                SDL_Color pixel = getPixel(x, y);
                                R.push_back(pixel.r);
                            }
                        }
                        if(tryb & 0x01) {
                            vector<token8> lz77 = LZ77Kompresja(R, R.size());
                            for(int i = 0; i < lz77.size(); i++) {
                                wyjscie.write((char*)&lz77[i], sizeof(token8));
                            }
                            lz77.clear();
                        } else {
                            for(int i = 0; i < R.size(); i++) {
                                wyjscie.write((char*)&R[i], sizeof(Uint8));
                            }
                        }
                        R.clear();
                    }
                }
            } else {
                if(tryb & 0x80) {
                    vector<Uint16> obraz;
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            obraz.push_back(getRGB565(x, y));
                        }
                    }
                    if(tryb & 0x01) {
                        vector<token16> lz77 = LZ77Kompresja(obraz, obraz.size());
                        for(int i = 0; i < lz77.size(); i++) {
                            wyjscie.write((char*)&lz77[i], sizeof(token16));
                        }
                        lz77.clear();
                    } else {
                        for(int i = 0; i < obraz.size(); i++) {
                            wyjscie.write((char*)&obraz[i], sizeof(Uint16));
                        }
                    }
                    obraz.clear();
                } else {
                    vector<Uint8> obraz;
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            obraz.push_back(getRGB565_(x, y).g);
                        }
                    }
                    if(tryb & 0x01) {
                        vector<token8> lz77 = LZ77Kompresja(obraz, obraz.size());
                        for(int i = 0; i < lz77.size(); i++) {
                            wyjscie.write((char*)&lz77[i], sizeof(token8));
                        }
                        lz77.clear();
                    } else {
                        for(int i = 0; i < obraz.size(); i++) {
                            wyjscie.write((char*)&obraz[i], sizeof(Uint8));
                        }
                    }
                    obraz.clear();
                }
            }
        }
    }

    cout << "Zapisano plik " << a << ".z21" << endl << endl;

    wyjscie.close();
}


void OdczytajPlik(string filename){
    czyscEkran(0,0,0);

    /** bits: [hasColor] [bits] [dithering] [prediction] [colorScheme] [subsampling] [dct] [lz77]
     * bits: 0 = 16-bit / 1 = 24-bit
     * colorScheme: 0 = rgb / 1 = hsl
     */
    Uint8 readMode;

    SDL_Color kolor;
    Uint8 kolorUint8;
    int counter = 0;
    char identyfikator[] = "    ";
    Uint16 szerokoscObrazka = 0;
    Uint16 wysokoscObrazka = 0;
    Uint8 rozmiarPalety = 0;
    Uint8 bitShiftedValue;
    Uint8 extractedBits, extractedBits2;
    Uint8 extractedBitsShifted;
    int offset = 0;

    cout<<"\nOdczytujemy plik '"<< filename <<".z21'\n\n";
    ifstream wejscie((filename + ".z21").c_str() ,ios::binary);
    wejscie.read((char*)&identyfikator,sizeof(char)*4);
    wejscie.read((char*)&szerokoscObrazka,sizeof(Uint16));
    wejscie.read((char*)&wysokoscObrazka,sizeof(Uint16));
    wejscie.read((char*)&readMode,sizeof(Uint8));

    cout << "Odczyt z pliku\n";
    cout << "Id: " << identyfikator << endl;
    cout << "Szerokość: " << szerokoscObrazka << endl;
    cout << "Wysokość: " << wysokoscObrazka << endl;
    cout << "Tryb: ";
    if(readMode & 0x80) {
        cout << "\tKolor" << endl;
    } else {
        cout << "\tCzarno-biały" << endl;
    }
    if(readMode & 0x40) {
        cout << "\t\t24-bit" << endl;
    } else {
        cout << "\t\t16-bit" << endl;
    }
    if(readMode & 0x20) {
        cout << "\t\tDithering: ON" << endl;
    } else {
        cout << "\t\tDithering: OFF" << endl;
    }
    if(readMode & 0x10) {
        cout << "\t\tFiltr Różnicowy: ON" << endl;
    } else {
        cout << "\t\tFiltr Różnicowy: OFF" << endl;
    }
    if(readMode & 0x08) {
        cout << "\t\tModel Barwny: HSL" << endl;
    } else {
        cout << "\t\tModel Barwny: RGB" << endl;
    }
    if(readMode & 0x04) {
        cout << "\t\tPodpróbkowanie 420: ON" << endl;
    } else {
        cout << "\t\tPodpróbkowanie 420: OFF" << endl;
    }
    if(readMode & 0x02) {
        cout << "\t\tDCT: ON" << endl;
    } else {
        cout << "\t\tDCT: OFF" << endl;
    }
    if(readMode & 0x01) {
        cout << "\t\tLZ77: ON" << endl;
    } else {
        cout << "\t\tLZ77: OFF" << endl;
    }

    if(readMode & 0x80) /* COLOR */ {
        if(readMode & 0x40) /* 24-bit */ {
            if(readMode & 0x02) /* DCT */ {
                float mnoznik;
                float buff;
                Uint8 uint8buff;
                vector<float> firstFactors;
                vector<Uint8> rest;
                wejscie.read((char*)&mnoznik,sizeof(float));
                for(int i = 0; i < 3000; i++) {
                    wejscie.read((char*)&buff,sizeof(float));
                    firstFactors.push_back(buff);
                }
                while(!wejscie.eof()) {
                    wejscie.read((char*)&uint8buff, sizeof(Uint8));
                    rest.push_back(uint8buff);
                }
                DCToutput input;
                input.mnoznik = mnoznik;
                input.pierwszeWspolczynniki = firstFactors;
                input.reszta = rest;
                vector<macierz> outputAllBlocks = DCTDekompresja(input, readMode);
                // wyjscie danych na ekran
                if(readMode & 0x08) /* HSL */ {
                    // po wysokosci -> 25 przebiegow
                    for(int k = 0; k < (wysokosc / 2 / rozmiarBloku); k++) {
                        // po szerokosci -> 40 przebiegow
                        for (int l = 0; l < (szerokosc / 2 / rozmiarBloku); l++) {
                            // pobranie danych w bloku
                            for (int x = 0; x < rozmiarBloku; x++) {
                                for (int y = 0; y < rozmiarBloku; y++) {
                                    SDL_Color pixel = HSLtoRGB(outputAllBlocks[(k * 40) + l].dane[x][y],
                                                               outputAllBlocks[(k * 40) + l + 1000].dane[x][y],
                                                               outputAllBlocks[(k * 40) + l + 2000].dane[x][y]);
                                    setPixel(x + (l * rozmiarBloku),
                                             y + (k * rozmiarBloku),
                                             pixel.r,
                                             pixel.g,
                                             pixel.b);

                                }
                            }
                        }
                    }
                } else /* RGB */ {
                    // po wysokosci -> 25 przebiegow
                    for(int k = 0; k < (wysokosc / 2 / rozmiarBloku); k++) {
                        // po szerokosci -> 40 przebiegow
                        for (int l = 0; l < (szerokosc / 2 / rozmiarBloku); l++) {
                            // pobranie danych w bloku
                            for (int x = 0; x < rozmiarBloku; x++) {
                                for (int y = 0; y < rozmiarBloku; y++) {
                                    setPixel(x + (l * rozmiarBloku),
                                             y + (k * rozmiarBloku),
                                             outputAllBlocks[(k * 40) + l].dane[x][y],
                                             outputAllBlocks[(k * 40) + l + 1000].dane[x][y],
                                             outputAllBlocks[(k * 40) + l + 2000].dane[x][y]);
                                }
                            }
                        }
                    }
                }
            } else /* no DCT */ {
                if(readMode & 0x01) /* LZ77 */ {
                    token8 token8buff;
                    vector<token8> combinedInput;
                    while(!wejscie.eof()) {
                        wejscie.read((char*)&token8buff, sizeof(token8));
                        combinedInput.push_back(token8buff);
                    }
                    vector<Uint8> decompressedCombinedInput = LZ77Dekompresja(combinedInput);
                    if(readMode & 0x10) /* Prediction */ {
                        vector<Uint8> RH;
                        vector<Uint8> GS;
                        vector<Uint8> BL;
                        for(int i = 0; i < 64000; i++) {
                            RH.push_back(decompressedCombinedInput[i]);
                            GS.push_back(decompressedCombinedInput[i + 64000]);
                            BL.push_back(decompressedCombinedInput[i + 128000]);
                        }
                        vector<Uint8> RHrp = reverseFiltrRoznicowy(RH);
                        vector<Uint8> GSrp = reverseFiltrRoznicowy(GS);
                        vector<Uint8> BLrp = reverseFiltrRoznicowy(BL);
                        SDL_Color pixel;
                        int vectorIndex = 0;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for (int y = 0; y < wysokosc / 2; y++) {
                                if (readMode & 0x08) {
                                    pixel = HSLtoRGB(RHrp[vectorIndex], GSrp[vectorIndex], BLrp[vectorIndex]);
                                } else {
                                    pixel.r = RHrp[vectorIndex];
                                    pixel.g = GSrp[vectorIndex];
                                    pixel.b = BLrp[vectorIndex];
                                }
                                setPixel(x, y, pixel.r, pixel.g, pixel.b);
                                vectorIndex++;
                            }
                        }
                    } else {
                        int vectorIndex = 0;
                        SDL_Color pixel;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                if(readMode & 0x08) {
                                    pixel = HSLtoRGB(decompressedCombinedInput[vectorIndex],
                                                     decompressedCombinedInput[vectorIndex + 64000],
                                                     decompressedCombinedInput[vectorIndex + 128000]);
                                } else {
                                    pixel.r = decompressedCombinedInput[vectorIndex];
                                    pixel.g = decompressedCombinedInput[vectorIndex + 64000];
                                    pixel.b = decompressedCombinedInput[vectorIndex + 128000];
                                }
                                setPixel(x, y, pixel.r, pixel.g, pixel.b);
                                vectorIndex++;
                            }
                        }
                    }
                } else /* no LZ77 */ {
                    Uint8 buff;
                    vector<Uint8> combinedInput;
                    while(!wejscie.eof()) {
                        wejscie.read((char*)&buff, sizeof(Uint8));
                        combinedInput.push_back(buff);
                    }
                    vector<Uint8> factor1;
                    vector<Uint8> factor2;
                    vector<Uint8> factor3;
                    for(int i = 0; i < 64000; i++) {
                        factor1.push_back(combinedInput[i]);
                        factor2.push_back(combinedInput[i+64000]);
                        factor3.push_back(combinedInput[i+128000]);
                    }
                    if(readMode & 0x10) /* Prediction */ {
                        vector<Uint8> factor1reversed = reverseFiltrRoznicowy(factor1);
                        vector<Uint8> factor2reversed = reverseFiltrRoznicowy(factor2);
                        vector<Uint8> factor3reversed = reverseFiltrRoznicowy(factor3);
                        int vectorIndex = 0;
                        SDL_Color pixel;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                if(readMode & 0x08) {
                                    pixel = HSLtoRGB(factor1reversed[vectorIndex],
                                                     factor2reversed[vectorIndex],
                                                     factor3reversed[vectorIndex]);
                                } else {
                                    pixel.r = factor1reversed[vectorIndex];
                                    pixel.g = factor2reversed[vectorIndex];
                                    pixel.b = factor3reversed[vectorIndex];
                                }
                                setPixel(x, y, pixel.r, pixel.g, pixel.b);
                                vectorIndex++;
                            }
                        }
                    } else /* no prediction */ {
                        int vectorIndex = 0;
                        SDL_Color pixel;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                if(readMode & 0x08) {
                                    pixel = HSLtoRGB(factor1[vectorIndex],
                                                     factor2[vectorIndex],
                                                     factor3[vectorIndex]);
                                } else {
                                    pixel.r = factor1[vectorIndex];
                                    pixel.g = factor2[vectorIndex];
                                    pixel.b = factor3[vectorIndex];
                                }
                                setPixel(x, y, pixel.r, pixel.g, pixel.b);
                                vectorIndex++;
                            }
                        }
                    }
                }
            }
        } else /* 16-bit */ {
            if(readMode & 0x01) /* LZ77 */ {
                token16 buff;
                vector<token16> inputToken;
                while(!wejscie.eof()) {
                    wejscie.read((char*)&buff, sizeof(token16));
                    inputToken.push_back(buff);
                }
                vector<Uint16> decompressedInput = LZ77Dekompresja(inputToken);
                if(readMode & 0x10) /* Prediction */ {
                    vector<Uint16> decompressedInputNoPrediction = reverseFiltrRoznicowy(decompressedInput);
                    int vectorIndex = 0;
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            setRGB565(x, y, decompressedInputNoPrediction[vectorIndex]);
                            vectorIndex++;
                        }
                    }
                } else /* no prediction */ {
                    int vectorIndex = 0;
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            setRGB565(x, y, decompressedInput[vectorIndex]);
                            vectorIndex++;
                        }
                    }
                }
            } else /* no LZ77 */ {
                Uint16 buff;
                vector<Uint16> inputUint16;
                while(!wejscie.eof()) {
                    wejscie.read((char*)&buff, sizeof(Uint16));
                    inputUint16.push_back(buff);
                }
                if(readMode & 0x10) /* Prediction */ {
                    vector<Uint16> decompressedInputNoPrediction = reverseFiltrRoznicowy(inputUint16);
                    int vectorIndex = 0;
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            setRGB565(x, y, decompressedInputNoPrediction[vectorIndex]);
                            vectorIndex++;
                        }
                    }
                } else /* no prediction */ {
                    int vectorIndex = 0;
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            setRGB565(x, y, inputUint16[vectorIndex]);
                            vectorIndex++;
                        }
                    }
                }
            }
        }
    } else /* BW */{
        if(readMode & 0x40) /* 24-bit */ {
            if(readMode & 0x02) /* DCT */ {
                float mnoznik;
                float buff;
                Uint8 uint8buff;
                vector<float> firstFactors;
                vector<Uint8> rest;
                wejscie.read((char*)&mnoznik,sizeof(float));
                for(int i = 0; i < 1000; i++) {
                    wejscie.read((char*)&buff,sizeof(float));
                    firstFactors.push_back(buff);
                }
                while(!wejscie.eof()) {
                    wejscie.read((char*)&uint8buff, sizeof(Uint8));
                    rest.push_back(uint8buff);
                }
                DCToutput input;
                input.mnoznik = mnoznik;
                input.pierwszeWspolczynniki = firstFactors;
                input.reszta = rest;
                vector<macierz> outputAllBlocks = DCTDekompresja(input, readMode);
                // wyjscie na ekran
                // po wysokosci -> 25 przebiegow
                for(int k = 0; k < (wysokosc / 2 / rozmiarBloku); k++) {
                    // po szerokosci -> 40 przebiegow
                    for (int l = 0; l < (szerokosc / 2 / rozmiarBloku); l++) {
                        // pobranie danych w bloku
                        for (int x = 0; x < rozmiarBloku; x++) {
                            for (int y = 0; y < rozmiarBloku; y++) {
                                setPixel(x + (l * rozmiarBloku),
                                         y + (k * rozmiarBloku),
                                         outputAllBlocks[(k * 40) + l].dane[x][y],
                                         outputAllBlocks[(k * 40) + l].dane[x][y],
                                         outputAllBlocks[(k * 40) + l].dane[x][y]);
                            }
                        }
                    }
                }
            } else /* no DCT */ {
                if(readMode & 0x01) /* LZ77 */ {
                    token8 token8buff;
                    vector<token8> combinedInput;
                    while(!wejscie.eof()) {
                        wejscie.read((char*)&token8buff, sizeof(token8));
                        combinedInput.push_back(token8buff);
                    }
                    vector<Uint8> decompressedCombinedInput = LZ77Dekompresja(combinedInput);
                    if(readMode & 0x10) /* Prediction */ {
                        vector<Uint8> RHrp = reverseFiltrRoznicowy(decompressedCombinedInput);
                        SDL_Color pixel;
                        int vectorIndex = 0;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for (int y = 0; y < wysokosc / 2; y++) {
                                if (readMode & 0x08) {
                                    pixel = HSLtoRGB(0, 0, RHrp[vectorIndex]);
                                } else {
                                    pixel.r = RHrp[vectorIndex];
                                    pixel.g = RHrp[vectorIndex];
                                    pixel.b = RHrp[vectorIndex];
                                }
                                setPixel(x, y, pixel.r, pixel.g, pixel.b);
                                vectorIndex++;
                            }
                        }
                    } else {
                        SDL_Color pixel;
                        int vectorIndex = 0;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for (int y = 0; y < wysokosc / 2; y++) {
                                if (readMode & 0x08) {
                                    pixel = HSLtoRGB(0,
                                                     0,
                                                     decompressedCombinedInput[vectorIndex]);
                                } else {
                                    pixel.r = decompressedCombinedInput[vectorIndex];
                                    pixel.g = decompressedCombinedInput[vectorIndex];
                                    pixel.b = decompressedCombinedInput[vectorIndex];
                                }
                                setPixel(x, y, pixel.r, pixel.g, pixel.b);
                                vectorIndex++;
                            }
                        }
                    }
                } else /* no LZ77 */ {
                    Uint8 buff;
                    vector<Uint8> combinedInput;
                    while(!wejscie.eof()) {
                        wejscie.read((char*)&buff, sizeof(Uint8));
                        combinedInput.push_back(buff);
                    }
                    if(readMode & 0x10) /* Prediction */ {
                        vector<Uint8> RHrp = reverseFiltrRoznicowy(combinedInput);
                        SDL_Color pixel;
                        int vectorIndex = 0;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for (int y = 0; y < wysokosc / 2; y++) {
                                if (readMode & 0x08) {
                                    pixel = HSLtoRGB(0, 0, RHrp[vectorIndex]);
                                } else {
                                    pixel.r = RHrp[vectorIndex];
                                    pixel.g = RHrp[vectorIndex];
                                    pixel.b = RHrp[vectorIndex];
                                }
                                setPixel(x, y, pixel.r, pixel.g, pixel.b);
                                vectorIndex++;
                            }
                        }
                    } else {
                        SDL_Color pixel;
                        int vectorIndex = 0;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for (int y = 0; y < wysokosc / 2; y++) {
                                if (readMode & 0x08) {
                                    pixel = HSLtoRGB(0,
                                                     0,
                                                     combinedInput[vectorIndex]);
                                } else {
                                    pixel.r = combinedInput[vectorIndex];
                                    pixel.g = combinedInput[vectorIndex];
                                    pixel.b = combinedInput[vectorIndex];
                                }
                                setPixel(x, y, pixel.r, pixel.g, pixel.b);
                                vectorIndex++;
                            }
                        }
                    }
                }
            }
        } else /* 16-bit */ {
            if(readMode & 0x01) /* LZ77 */ {
                token8 buff;
                vector<token8> inputToken;
                while(!wejscie.eof()) {
                    wejscie.read((char*)&buff, sizeof(token8));
                    inputToken.push_back(buff);
                }
                vector<Uint8> decompressedInput = LZ77Dekompresja(inputToken);
                if(readMode & 0x10) /* Prediction */ {
                    vector<Uint8> decompressedInputNoPrediction = reverseFiltrRoznicowy(decompressedInput);
                    int vectorIndex = 0;
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            setRGB565(x, y, decompressedInputNoPrediction[vectorIndex] >> 1,
                                      decompressedInputNoPrediction[vectorIndex],
                                      decompressedInputNoPrediction[vectorIndex] >> 1);
                            vectorIndex++;
                        }
                    }
                } else /* no prediction */ {
                    int vectorIndex = 0;
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            setRGB565(x, y, decompressedInput[vectorIndex] >> 1,
                                      decompressedInput[vectorIndex],
                                      decompressedInput[vectorIndex] >> 1);
                            vectorIndex++;
                        }
                    }                }
            } else /* no LZ77 */ {
                Uint8 buff;
                vector<Uint8> inputUint8;
                while(!wejscie.eof()) {
                    wejscie.read((char*)&buff, sizeof(Uint8));
                    inputUint8.push_back(buff);
                }
                if(readMode & 0x10) /* Prediction */ {
                    vector<Uint8> decompressedInputNoPrediction = reverseFiltrRoznicowy(inputUint8);
                    int vectorIndex = 0;
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            setRGB565(x, y, decompressedInputNoPrediction[vectorIndex] >> 1,
                                      decompressedInputNoPrediction[vectorIndex],
                                      decompressedInputNoPrediction[vectorIndex] >> 1);
                            vectorIndex++;
                        }
                    }
                } else /* no prediction */ {
                    int vectorIndex = 0;
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            setRGB565(x, y, inputUint8[vectorIndex] >> 1,
                                      inputUint8[vectorIndex],
                                      inputUint8[vectorIndex] >> 1);
                            vectorIndex++;
                        }
                    }
                }
            }
        }
    }

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

