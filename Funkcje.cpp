// podstawowe funkcje
#include "Funkcje.h"
#include "Zmienne.h"
#include "Paleta.h"
#include "MedianCut.h"
#include "Pliki.h"
#include "Modele.h"
#include "Kompresja.h"
#include <math.h>
#include <vector>

vector<Uint8> zczytajDaneBW(){
    vector<Uint8> output;
    for(int x = 0; x < szerokosc/2; x++){
        for(int y = 0; y < wysokosc/2; y++){
            SDL_Color color = getPixel(x, y);
            output.push_back(z24RGBna8BW(color));
        }
    }

    return output;
}

void filtrRoznicowy(Uint8 tabela[320][200]){
    int roznica;
    for(int x =0;x< szerokosc/2; x++){
        for(int y=0; y <wysokosc/2; y++){
            if (x == 0) {
                tabelaFiltr[0][y] = tabela[0][y];
            } else {
                roznica = tabela[x][y] - tabela[x-1][y];
                tabelaFiltr[x][y] = roznica;
            }
        }
    }
}

void roznicaLinii(Uint8 tabela[320][200]){
    int roznica;
    for(int x =0;x< szerokosc/2; x++){
        for(int y=0; y <wysokosc/2; y++){
            if (y == 0) {
                tabelaFiltr[x][y] = tabela[x][y];
            } else {
                roznica = tabela[x][y] - tabela[x][y-1];
                tabelaFiltr[x][y] = roznica;
                cout << (int)tabela[x][y] << " - " << (int)tabela[x][y-1] << " = " << tabelaFiltr[x][y] << endl;
            }
        }
    }
}

void filtrUsredniajacy(Uint8 tabela[320][200]){
    int roznica;
    for(int x =0;x< szerokosc/2; x++){
        for(int y=0; y <wysokosc/2; y++){
            if (x == 0 || y == 0) {
                tabelaFiltr[x][y] = tabela[x][y];
            } else {
                roznica = tabela[x][y] - ( (tabela[x][y-1] + tabela[x-1][y]) / 2 );
                tabelaFiltr[x][y] = roznica;
                cout << (int)tabela[x][y] << " - ((" << (int)tabela[x][y-1] << " + " << (int)tabela[x-1][y] << " ) / 2 )  = " << tabelaFiltr[x][y] << endl;
            }
        }
    }
}

void filtrPaetha(Uint8 tabela[320][200]){
    int roznica;
    int paeth, p;
    for(int x =0;x< szerokosc/2; x++){
        for(int y=0; y <wysokosc/2; y++){
            if (x == 0 || y == 0) {
                tabelaFiltr[x][y] = tabela[x][y];
            } else {
                paeth = (int)tabela[x-1][y] + (int)tabela[x][y-1] - (int)tabela[x-1][y-1];
                p = min(min((int)abs(tabela[x-1][y] - paeth), (int)abs(tabela[x][y-1] - paeth)), (int)abs(tabela[x-1][y-1] - paeth));
                roznica = tabela[x][y] - p;
                tabelaFiltr[x][y] = roznica;
                cout << "Paeth: " << paeth << "  p: " << p << endl;
                cout << (int)tabela[x][y] << " - " << p << " = " << tabelaFiltr[x][y] << endl;
            }
        }
    }
}



void Funkcja1() {
    string filename;
    cout << "Podaj nazwę pliku, który chcesz zapisać: ";
    cin >> filename;

    vector<Uint8> array = zczytajDaneBW();

    LZWKompresja(array, array.size(), filename);

    SDL_UpdateWindowSurface(window);
}

void Funkcja2() {
    string filename;
    cout << "Podaj nazwę pliku, który chcesz otworzyć: ";
    cin >> filename;

    LZWDekompresja(filename);
    SDL_UpdateWindowSurface(window);

//    SDL_Color kolor, tymczasowyKolor, newNewColor;
//    Uint8 szary, nowySzary;
//    int tymczasowySzary;
//    int przesuniecie = 1;
//    float bledy[(szerokosc/2)+2][(wysokosc/2)+2];
//    memset(bledy, 0, sizeof(bledy));
//    int blad = 0;
//
//
//    for(int yy=0; yy<wysokosc/2; yy++) {
//        for (int xx = 0; xx < szerokosc / 2; xx++) {
//            kolor = getPixel(xx, yy);
//
//            szary = 0.299*kolor.r + 0.587*kolor.g + 0.114*kolor.b;
//            tymczasowySzary = szary + bledy[xx + przesuniecie][yy];
//            if(tymczasowySzary > 255) {
//                tymczasowySzary = 255;
//            }
//            if(tymczasowySzary < 0) {
//                tymczasowySzary = 0;
//            }
//
//            tymczasowyKolor.r = tymczasowySzary;
//            tymczasowyKolor.b = tymczasowySzary;
//            tymczasowyKolor.g = tymczasowySzary;
//
//            szary = z24RGBna7BW(kolor);
//            newNewColor = z7BWna24RGB(szary);
//
//            nowySzary = newNewColor.r;
//
//            blad = tymczasowySzary - nowySzary;
//
//            setPixel(xx + szerokosc/2, yy, newNewColor.r, newNewColor.g, newNewColor.b);
//
//            bledy[xx+1+przesuniecie][yy] += (blad*7.0/16.0);
//            bledy[xx-1+przesuniecie][yy+1] += (blad*3.0/16.0);
//            bledy[xx+przesuniecie][yy+1] += (blad*5.0/16.0);
//            bledy[xx-1+przesuniecie][yy+1] += (blad*1.0/16.0);
//        }
//    }

//    podprobkowanieYUV();

    SDL_UpdateWindowSurface(window);
}

void Funkcja3() {

//    ileKubelkow = 0;
//    int numer = 0;
//    Uint8 szary = 0;
//    SDL_Color kolor;
//    int indeks = 0;
//
//    for (int y=0;y<wysokosc/2;y++)
//    {
//        for (int x=0;x<szerokosc/2;x++)
//        {
//            kolor = getPixel(x,y);
//            szary = 0.299 * kolor.r + 0.587 * kolor.g + 0.114 * kolor.b;
//            obrazek[numer] = {szary,szary,szary};
//            numer++;
//        }
//    }
//    SDL_UpdateWindowSurface(window);
//    medianCutBW(0,numer-1,7);
//
//    for (int y=0;y<wysokosc/2;y++)
//    {
//        for (int x=0;x<szerokosc/2;x++)
//        {
//            kolor = getPixel(x,y);
//            szary = 0.299 * kolor.r + 0.587 * kolor.g + 0.114 * kolor.b;
//            indeks = znajdzSasiadaBW(szary);
//            setPixel(x + szerokosc/2, y, paleta7[indeks].r, paleta7[indeks].g, paleta7[indeks].b);
//
//        }
//    }
//    narysujPalete(paleta7);
//
//    for(int x = 0; x < 128; x++){
//        paleta7DoPliku[x] = paleta7[x];
//        //cout << "R:" << (int)paleta7DoPliku[x].r << "G:" << (int)paleta7DoPliku[x].g << "B:" << (int)paleta7DoPliku[x].b << endl;
//    }

    podprobkowanieYIQ();


    SDL_UpdateWindowSurface(window);
}

void Funkcja4() {

//    czyscPalete();
//    int indexKoloru;
//    SDL_Color kolor;
//    int flag;
//    for (int y = 0; y < wysokosc/2; y++) {
//        for (int x = 0; x < szerokosc/2; x++) {
//            kolor = getPixel(x, y);
//            flag = sprawdzKolor(kolor);
//            if (flag == -1) {
//                break;
//            }
//        }
//        if (flag == -1) {
//            break;
//        }
//    }
//    for (int y=0;y<wysokosc/2;y++)
//    {
//        for (int x=0;x<szerokosc/2;x++)
//        {
//            kolor = getPixel(x,y);
//            indexKoloru = znajdzSasiada(kolor);
//            setPixel(x + szerokosc/2, y, paleta7[indexKoloru].r, paleta7[indexKoloru].g, paleta7[indexKoloru].b);
//        }
//    }
//    for(int x = 0; x < 128; x++){
//        paleta7DoPliku[x] = paleta7[x];
//        //cout << "R:" << (int)paleta7DoPliku[x].r << "G:" << (int)paleta7DoPliku[x].g << "B:" << (int)paleta7DoPliku[x].b << endl;
//    }
//    narysujPalete(paleta7);

    podprobkowanieYCBCR();


    SDL_UpdateWindowSurface(window);
}

void Funkcja5() {

//    ileKubelkow = 0;
//    int numer = 0;
//    SDL_Color kolor;
//    int indeks = 0;
//
//    for (int y=0;y<wysokosc/2;y++)
//    {
//        for (int x=0;x<szerokosc/2;x++)
//        {
//            kolor = getPixel(x,y);
//            obrazek[numer] = kolor;
//            numer++;
//        }
//    }
//    medianCut(0,numer-1,7);
//
//    for (int y=0;y<wysokosc/2;y++)
//    {
//        for (int x=0;x<szerokosc/2;x++)
//        {
//            kolor = getPixel(x,y);
//            indeks = znajdzSasiada(kolor);
//            setPixel(x + szerokosc/2, y, paleta7[indeks].r, paleta7[indeks].g, paleta7[indeks].b);
//        }
//    }
//    narysujPalete(paleta7);
//    for(int x = 0; x < 128; x++){
//        paleta7DoPliku[x] = paleta7[x];
//        //cout << "R:" << (int)paleta7DoPliku[x].r << "G:" << (int)paleta7DoPliku[x].g << "B:" << (int)paleta7DoPliku[x].b << endl;
//    }

    podprobkowanieHSL();

    SDL_UpdateWindowSurface(window);
}

void Funkcja6() {

    SDL_Color kolor, tymczasowyKolor, newNewColor;
    Uint8 R, G, B, nowyR, nowyG, nowyB, kolor7bit;
    int tempR, tempG, tempB;
    int przesuniecie = 1;
    float bledyR[(szerokosc/2)+2][(wysokosc/2)+2];
    float bledyG[(szerokosc/2)+2][(wysokosc/2)+2];
    float bledyB[(szerokosc/2)+2][(wysokosc/2)+2];
    memset(bledyR, 0, sizeof(bledyR));
    memset(bledyG, 0, sizeof(bledyG));
    memset(bledyB, 0, sizeof(bledyB));
    int bladR, bladG, bladB = 0;


    for(int yy=0; yy<wysokosc/2; yy++) {
        for (int xx = 0; xx < szerokosc / 2; xx++) {
            kolor = getPixel(xx, yy);

            R = kolor.r;
            G = kolor.g;
            B = kolor.b;

            tempR = R + bledyR[xx + przesuniecie][yy];
            tempG = G + bledyG[xx + przesuniecie][yy];
            tempB = B + bledyB[xx + przesuniecie][yy];
            if(tempR > 255) {
                tempR = 255;
            }
            if(tempR < 0) {
                tempR = 0;
            }
            if(tempG > 255) {
                tempG = 255;
            }
            if(tempG < 0) {
                tempG = 0;
            }
            if(tempB > 255) {
                tempB = 255;
            }
            if(tempB < 0) {
                tempB = 0;
            }

            tymczasowyKolor.r = tempR;
            tymczasowyKolor.g = tempG;
            tymczasowyKolor.b = tempB;
            kolor7bit = z24RGBna7RGB(tymczasowyKolor);
            newNewColor = z7RGBna24RGB(kolor7bit);

            nowyR = newNewColor.r;
            nowyG = newNewColor.g;
            nowyB = newNewColor.b;

            bladR = tempR - nowyR;
            bladG = tempG - nowyG;
            bladB = tempB - nowyB;

            setPixel(xx + szerokosc/2, yy, nowyR, nowyG, nowyB);

            bledyR[xx+1+przesuniecie][yy] += (bladR*7.0/16.0);
            bledyR[xx-1+przesuniecie][yy+1] += (bladR*3.0/16.0);
            bledyR[xx+przesuniecie][yy+1] += (bladR*5.0/16.0);
            bledyR[xx-1+przesuniecie][yy+1] += (bladR*1.0/16.0);

            bledyG[xx+1+przesuniecie][yy] += (bladG*7.0/16.0);
            bledyG[xx-1+przesuniecie][yy+1] += (bladG*3.0/16.0);
            bledyG[xx+przesuniecie][yy+1] += (bladG*5.0/16.0);
            bledyG[xx-1+przesuniecie][yy+1] += (bladG*1.0/16.0);

            bledyB[xx+1+przesuniecie][yy] += (bladB*7.0/16.0);
            bledyB[xx-1+przesuniecie][yy+1] += (bladB*3.0/16.0);
            bledyB[xx+przesuniecie][yy+1] += (bladB*5.0/16.0);
            bledyB[xx-1+przesuniecie][yy+1] += (bladB*1.0/16.0);
        }
    }

    SDL_UpdateWindowSurface(window);
}

void Funkcja7() {
    string nazwa;
    int tryb;
    cout << "Podaj nazwe dla pliku " << endl;
    cin >> nazwa;
    cout << "Podaj w jakim trybie go zapisać: "
            "\n1 - RGB888\n2 - YUV888\n3 - YIQ888\n4 - YCbCr888\n5 - HSL888"
            "\n6 - RGB555\n7 - RGB565" << endl;
    cin >> tryb;
    //ZapiszPlik(nazwa);
    ZapiszModel(nazwa, tryb + 9);
}

void Funkcja8() {
    string nazwa;
    cout << "\nPodaj nazwe pliku, ktory chcesz otworzyc " << endl;
    cin >> nazwa;
    OdczytajModel(nazwa);
    SDL_UpdateWindowSurface(window);

}

void Funkcja9() {

    Uint8 tabela[320][200];
    for(int x =0;x< szerokosc/2; x=x+2){
        for(int y=0; y <wysokosc/2; y=y+2){
            tabela[x][y] = getPixel(x,y).r;
        }
    }
    filtrPaetha(tabela);
    SDL_UpdateWindowSurface(window);
}

Uint8 z24RGBna7RGB(SDL_Color kolor) {
    Uint8 R,G,B,kolor7bit;
    int nowyR, nowyG, nowyB;

    R=kolor.r;
    G=kolor.g;
    B=kolor.b;

    nowyR=round(R*3.0/255.0);
    nowyG=round(G*7.0/255.0);
    nowyB=round(B*3.0/255.0);

    kolor7bit = (nowyR<<6) | (nowyG<<3) | (nowyB<<1);

    return kolor7bit;
}

SDL_Color z7RGBna24RGB(Uint8 kolor7bit){
    //RRGGGBB0
    int R,G,B,nowyR,nowyG,nowyB;
    SDL_Color kolor24RGB;
    nowyR = (kolor7bit & (0b11000000)) >> 6;
    nowyG = (kolor7bit & (0b00111000)) >> 3;
    nowyB = (kolor7bit & (0b00000110)) >> 1;


    R= nowyR*255.0/3.0;
    G= nowyG*255.0/7.0;
    B= nowyB*255.0/3.0;

    kolor24RGB.r = R;
    kolor24RGB.g = G;
    kolor24RGB.b = B;

    return kolor24RGB;
}


Uint8 z24RGBna7BW (SDL_Color kolor) {

    Uint8 szary8bit = 0.299*kolor.r + 0.587*kolor.g + 0.114*kolor.b;
    Uint8 szary7bit = round(szary8bit * 127.0/255.0);

    return szary7bit;

}

SDL_Color z7BWna24RGB (Uint8 szary7bit) {
    SDL_Color kolor;
    Uint8 szary8bit = szary7bit*255.0/127.0;
    kolor.r = szary8bit;
    kolor.g = szary8bit;
    kolor.b = szary8bit;

    return kolor;

}

Uint8 z24RGBna8BW (SDL_Color kolor) {

    Uint8 szary8bit = 0.299*kolor.r + 0.587*kolor.g + 0.114*kolor.b;
    return szary8bit;

}

SDL_Color z8BWna24RGB (Uint8 szary8bit) {
    SDL_Color kolor;
    kolor.r = szary8bit;
    kolor.g = szary8bit;
    kolor.b = szary8bit;

    return kolor;
}

void zbierzKoloryZObrazka() {
    int n = 0;
    for(int l = 0; l < 25; l++) {
        for (int k = 0; k < 40; k++) {
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    SDL_Color kolor = getPixel(k*8+j, l*8+i);
                    obrazWTablicy[n] = z24RGBna7RGB(kolor);
                    n++;
                }
            }
        }
    }
}


void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
  if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc))
  {
    /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

    /* Pobieramy informację ile bajtów zajmuje jeden piksel */
    int bpp = screen->format->BytesPerPixel;

    /* Obliczamy adres piksela */
    Uint8 *p1 = (Uint8 *)screen->pixels + (y*2) * screen->pitch + (x*2) * bpp;
    Uint8 *p2 = (Uint8 *)screen->pixels + (y*2+1) * screen->pitch + (x*2) * bpp;
    Uint8 *p3 = (Uint8 *)screen->pixels + (y*2) * screen->pitch + (x*2+1) * bpp;
    Uint8 *p4 = (Uint8 *)screen->pixels + (y*2+1) * screen->pitch + (x*2+1) * bpp;

    /* Ustawiamy wartość piksela, w zależnoœci od formatu powierzchni*/
    switch(bpp)
    {
        case 1: //8-bit
            *p1 = pixel;
            *p2 = pixel;
            *p3 = pixel;
            *p4 = pixel;
            break;

        case 2: //16-bit
            *(Uint16 *)p1 = pixel;
            *(Uint16 *)p2 = pixel;
            *(Uint16 *)p3 = pixel;
            *(Uint16 *)p4 = pixel;
            break;

        case 3: //24-bit
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p1[0] = (pixel >> 16) & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = pixel & 0xff;
                p2[0] = (pixel >> 16) & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = pixel & 0xff;
                p3[0] = (pixel >> 16) & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = pixel & 0xff;
                p4[0] = (pixel >> 16) & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = pixel & 0xff;
            } else {
                p1[0] = pixel & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = (pixel >> 16) & 0xff;
                p2[0] = pixel & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = (pixel >> 16) & 0xff;
                p3[0] = pixel & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = (pixel >> 16) & 0xff;
                p4[0] = pixel & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32 *)p1 = pixel;
            *(Uint32 *)p2 = pixel;
            *(Uint32 *)p3 = pixel;
            *(Uint32 *)p4 = pixel;
            break;

        }
    }
}

void setPixelSurface(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
  if ((x>=0) && (x<szerokosc*2) && (y>=0) && (y<wysokosc*2))
  {
    /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

    /* Pobieramy informację ile bajtów zajmuje jeden piksel */
    int bpp = screen->format->BytesPerPixel;

    /* Obliczamy adres piksela */
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    /* Ustawiamy wartość piksela, w zależności od formatu powierzchni*/
    switch(bpp)
    {
        case 1: //8-bit
            *p = pixel;
            break;

        case 2: //16-bit
            *(Uint16 *)p = pixel;
            break;

        case 3: //24-bit
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32 *)p = pixel;
            break;
        }
    }
}

SDL_Color getPixel(int x, int y) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc)) {
        //określamy pozycję
        char* pPosition=(char*)screen->pixels ;

        //przesunięcie względem y
        pPosition+=(screen->pitch*y*2) ;

        //przesunięcie względem x
        pPosition+=(screen->format->BytesPerPixel*x*2);

        //kopiujemy dane piksela
        memcpy(&col, pPosition, screen->format->BytesPerPixel);

        //konwertujemy kolor
        SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}

SDL_Color getPixelSurface(int x, int y, SDL_Surface *surface) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc)) {
        //określamy pozycję
        char* pPosition=(char*)surface->pixels ;

        //przesunięcie względem y
        pPosition+=(surface->pitch*y) ;

        //przesunięcie względem x
        pPosition+=(surface->format->BytesPerPixel*x);

        //kopiujemy dane piksela
        memcpy(&col, pPosition, surface->format->BytesPerPixel);

        //konwertujemy kolor
        SDL_GetRGB(col, surface->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}


void ladujBMP(char const* nazwa, int x, int y) {
    SDL_Surface* bmp = SDL_LoadBMP(nazwa);
    if (!bmp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
    }
    else
    {
        SDL_Color kolor;
        for (int yy=0; yy<bmp->h; yy++) {
			for (int xx=0; xx<bmp->w; xx++) {
				kolor = getPixelSurface(xx, yy, bmp);
				setPixel(xx, yy, kolor.r, kolor.g, kolor.b);
			}
        }
		SDL_FreeSurface(bmp);
        SDL_UpdateWindowSurface(window);
    }

}

void podprobkowanieYUV() {
    pixele4YUV pix4yuv, retYuv;
    SDL_Color pixA, pixB, pixC, pixD;
    for(int i = 0; i <= szerokosc / 2; i = i + 2) {
        for (int k = 0; k <= wysokosc / 2; k = k + 2) {
            pix4yuv.a = RGBtoYUV(i,k);
            pix4yuv.b = RGBtoYUV(i + 1,k);
            pix4yuv.c = RGBtoYUV(i,k + 1);
            pix4yuv.d = RGBtoYUV(i + 1, k + 1);

            // te funkcje sie nazywają 420, ale tak naprawde one tylko usredniaja wiec uzywam ich też w 4:1:1
            retYuv = YUV_420(YUV_420(pix4yuv, 'u'), 'v');
            pixA = YUVtoRGB(retYuv.a.Y, retYuv.a.U, retYuv.a.V);
            pixB = YUVtoRGB(retYuv.b.Y, retYuv.b.U, retYuv.b.V);
            pixC = YUVtoRGB(retYuv.c.Y, retYuv.c.U, retYuv.c.V);
            pixD = YUVtoRGB(retYuv.d.Y, retYuv.d.U, retYuv.d.V);
            setPixel(i + szerokosc / 2, k, pixA.r, pixA.g, pixA.b);
            setPixel(i + 1 + szerokosc / 2 , k , pixB.r, pixB.g, pixB.b);
            setPixel(i + szerokosc / 2, k + 1, pixC.r, pixC.g, pixC.b);
            setPixel(i + 1 + szerokosc / 2, k + 1, pixD.r, pixD.g, pixD.b);
        }
    }
}

void podprobkowanieYIQ() {
    pixele4YIQ pix4, ret;
    SDL_Color pixA, pixB, pixC, pixD;
    for(int i = 0; i <= szerokosc / 2; i = i + 2) {
        for (int k = 0; k <= wysokosc / 2; k = k + 2) {
            pix4.a = RGBtoYIQ(i,k);
            pix4.b = RGBtoYIQ(i + 1,k);
            pix4.c = RGBtoYIQ(i,k + 1);
            pix4.d = RGBtoYIQ(i + 1, k + 1);

            // te funkcje sie nazywają 420, ale tak naprawde one tylko usredniaja wiec uzywam ich też w 4:1:1
            ret = YIQ_420(YIQ_420(pix4, 'i'), 'q');
            pixA = YIQtoRGB(ret.a.Y, ret.a.I, ret.a.Q);
            pixB = YIQtoRGB(ret.b.Y, ret.b.I, ret.b.Q);
            pixC = YIQtoRGB(ret.c.Y, ret.c.I, ret.c.Q);
            pixD = YIQtoRGB(ret.d.Y, ret.d.I, ret.d.Q);
            setPixel(i + szerokosc / 2, k, pixA.r, pixA.g, pixA.b);
            setPixel(i + 1 + szerokosc / 2 , k , pixB.r, pixB.g, pixB.b);
            setPixel(i + szerokosc / 2, k + 1, pixC.r, pixC.g, pixC.b);
            setPixel(i + 1 + szerokosc / 2, k + 1, pixD.r, pixD.g, pixD.b);
        }
    }
}

void podprobkowanieYCBCR() {
    pixele4YCbCr pix4, ret;
    SDL_Color pixA, pixB, pixC, pixD;
    for(int i = 0; i <= szerokosc / 2; i = i + 2) {
        for (int k = 0; k <= wysokosc / 2; k = k + 2) {
            pix4.a = RGBtoYCbCr(i,k);
            pix4.b = RGBtoYCbCr(i + 1,k);
            pix4.c = RGBtoYCbCr(i,k + 1);
            pix4.d = RGBtoYCbCr(i + 1, k + 1);

            // te funkcje sie nazywają 420, ale tak naprawde one tylko usredniaja wiec uzywam ich też w 4:1:1
            ret = YCbCr_420(YCbCr_420(pix4, 'cb'), 'cr');
            pixA = YCbCrtoRGB(ret.a.Y, ret.a.Cb, ret.a.Cr);
            pixB = YCbCrtoRGB(ret.b.Y, ret.b.Cb, ret.b.Cr);
            pixC = YCbCrtoRGB(ret.c.Y, ret.c.Cb, ret.c.Cr);
            pixD = YCbCrtoRGB(ret.d.Y, ret.d.Cb, ret.d.Cr);
            setPixel(i + szerokosc / 2, k, pixA.r, pixA.g, pixA.b);
            setPixel(i + 1 + szerokosc / 2 , k , pixB.r, pixB.g, pixB.b);
            setPixel(i + szerokosc / 2, k + 1, pixC.r, pixC.g, pixC.b);
            setPixel(i + 1 + szerokosc / 2, k + 1, pixD.r, pixD.g, pixD.b);
        }
    }
}

void podprobkowanieHSL() {
    pixele4HSL pix4, ret;
    SDL_Color pixA, pixB, pixC, pixD;
    for(int i = 0; i <= szerokosc / 2; i = i + 2) {
        for (int k = 0; k <= wysokosc / 2; k = k + 2) {
            pix4.a = RGBtoHSL(i,k);
            pix4.b = RGBtoHSL(i + 1,k);
            pix4.c = RGBtoHSL(i,k + 1);
            pix4.d = RGBtoHSL(i + 1, k + 1);

            // te funkcje sie nazywają 420, ale tak naprawde one tylko usredniaja wiec uzywam ich też w 4:1:1
            ret = HSL_420(pix4, 'l');
            pixA = HSLtoRGB(ret.a.H, ret.a.S, ret.a.L);
            pixB = HSLtoRGB(ret.b.H, ret.b.S, ret.b.L);
            pixC = HSLtoRGB(ret.c.H, ret.c.S, ret.c.L);
            pixD = HSLtoRGB(ret.d.H, ret.d.S, ret.d.L);
            setPixel(i + szerokosc / 2, k, pixA.r, pixA.g, pixA.b);
            setPixel(i + 1 + szerokosc / 2 , k , pixB.r, pixB.g, pixB.b);
            setPixel(i + szerokosc / 2, k + 1, pixC.r, pixC.g, pixC.b);
            setPixel(i + 1 + szerokosc / 2, k + 1, pixD.r, pixD.g, pixD.b);
        }
    }
}


void czyscEkran(Uint8 R, Uint8 G, Uint8 B)
{
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
    SDL_UpdateWindowSurface(window);
}

