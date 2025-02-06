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

vector<Uint8> filtrRoznicowy(vector<Uint8> input){
    vector<Uint8> output;
    output.push_back(input[0]);
    for(int i = 1; i < input.size(); i++) {
        output.push_back(input[i] - input[i-1]);
    }
    return output;
}

vector<Uint8> reverseFiltrRoznicowy(vector<Uint8> input){
    vector<Uint8> output;
    output.push_back(input[0]);
    for(int i = 1; i < input.size(); i++) {
        output.push_back(input[i] + output[i-1]);
    }
    return output;
}

vector<Uint16> filtrRoznicowy(vector<Uint16> input){
    vector<Uint16> output;
    output.push_back(input[0]);
    for(int i = 1; i < input.size(); i++) {
        output.push_back(input[i] - input[i-1]);
    }
    return output;
}

vector<Uint16> reverseFiltrRoznicowy(vector<Uint16> input){
    vector<Uint16> output;
    output.push_back(input[0]);
    for(int i = 1; i < input.size(); i++) {
        output.push_back(input[i] + output[i-1]);
    }
    return output;
}

void displayMainMenu() {
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
}

// USTAWIA TRYB color / bw
void Funkcja1() {
    SDL_Event event;
    cout << "\n\n\tWybierz tryb:\n1. Czarno-biały\n2. Kolorowy\n0. Zatwierdź\n";
    while (SDL_WaitEvent(&event)) {
        // sprawdzamy czy pojawiło się zdarzenie
        switch (event.type) {
            // sprawdzamy czy został wciśnięty klawisz
            case SDL_KEYDOWN: {
                // jeśli tryb bw
                if (event.key.keysym.sym == SDLK_1) {
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            Uint8 bw = z24RGBna8BW(getPixel(x, y));
                            SDL_Color bwSDL = z8BWna24RGB(bw);
                            setPixel(x + szerokosc / 2, y, bwSDL.r, bwSDL.g, bwSDL.b);
                        }
                    }
                    if(tryb & 0x80) {
                        tryb -= 0b10000000;
                    }
                    SDL_UpdateWindowSurface(window);
                    break;
                }
                // jeśli tryb color
                if (event.key.keysym.sym == SDLK_2) {
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            SDL_Color pixel = getPixel(x, y);
                            setPixel(x + szerokosc / 2, y, pixel.r, pixel.g, pixel.b);
                        }
                    }
                    if(!(tryb & 0x80)) {
                        tryb += 0b10000000;
                    }
                    SDL_UpdateWindowSurface(window);
                    break;
                }
                // jeśli zatwierdź
                if (event.key.keysym.sym == SDLK_0) {
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            SDL_Color pixel = getPixel(x + szerokosc / 2, y);
                            setPixel(x, y, pixel.r, pixel.g, pixel.b);
                            setPixel(x + szerokosc / 2, y, 0, 0, 0);
                        }
                    }
                    SDL_UpdateWindowSurface(window);
                    displayMainMenu();
                    return;
                } else {
                    break;
                }
            }
        }
    }
}

// USTAWIA TRYB 16-bit / 24-bit
void Funkcja2() {
    SDL_Event event;
    cout << "\n\n\tWybierz tryb:\n1. 16-bit\n2. 24-bit\n0. Zatwierdź\n";
    while (SDL_WaitEvent(&event)) {
        // sprawdzamy czy pojawiło się zdarzenie
        switch (event.type) {
            // sprawdzamy czy został wciśnięty klawisz
            case SDL_KEYDOWN: {
                // jeśli tryb 16-bit
                if (event.key.keysym.sym == SDLK_1) {
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            Uint16 pixel = getRGB565(x, y);
                            setRGB565(x + szerokosc / 2, y, pixel);
                        }
                    }
                    if(tryb & 0x40) {
                        tryb -= 0b01000000;
                    }
                    SDL_UpdateWindowSurface(window);
                    break;
                }
                // jeśli tryb 24-bit
                if (event.key.keysym.sym == SDLK_2) {
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            SDL_Color pixel = getPixel(x, y);
                            setPixel(x + szerokosc / 2, y, pixel.r, pixel.g, pixel.b);
                        }
                    }
                    if(!(tryb & 0x40)) {
                        tryb += 0b01000000;
                    }
                    SDL_UpdateWindowSurface(window);
                    break;
                }
                // jeśli zatwierdź
                if (event.key.keysym.sym == SDLK_0) {
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            SDL_Color pixel = getPixel(x + szerokosc / 2, y);
                            setPixel(x, y, pixel.r, pixel.g, pixel.b);
                            setPixel(x + szerokosc / 2, y, 0, 0, 0);
                        }
                    }
                    SDL_UpdateWindowSurface(window);
                    displayMainMenu();
                    return;
                } else {
                    break;
                }
            }
        }
    }
}

// WYBÓR MODELU BARWNEGO (RGB / HSL)
void Funkcja3() {
    if(!(tryb & 0x40)) {
        cout << "\n\nWybór modelu barwnego jest dostępny tylko w trybie 24-bit!\n\n";
        displayMainMenu();
        return;
    } else {
        SDL_Event event;
        cout << "\n\n\tWybierz model barwny:\n1. RGB888\n2. HSL888\n0. Zatwierdź\n";
        while (SDL_WaitEvent(&event)) {
            // sprawdzamy czy pojawiło się zdarzenie
            switch (event.type) {
                // sprawdzamy czy został wciśnięty klawisz
                case SDL_KEYDOWN: {
                    // jeśli RGB
                    if (event.key.keysym.sym == SDLK_1) {
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                SDL_Color pixel = getPixel(x, y);
                                setPixel(x + szerokosc / 2, y, pixel.r, pixel.g, pixel.b);
                            }
                        }
                        if(tryb & 0x08) {
                            tryb -= 0b00001000;
                        }
                        SDL_UpdateWindowSurface(window);
                        break;
                    }
                    // jeśli HSL
                    if (event.key.keysym.sym == SDLK_2) {
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                HSL hslPixel = RGBtoHSL(x, y);
                                SDL_Color pixel = HSLtoRGB(hslPixel.H, hslPixel.S, hslPixel.L);
                                setPixel(x + szerokosc / 2, y, pixel.r, pixel.g, pixel.b);
                            }
                        }
                        if(!(tryb & 0x08)) {
                            tryb += 0b00001000;
                        }
                        SDL_UpdateWindowSurface(window);
                        break;
                    }
                    // jeśli zatwierdź
                    if (event.key.keysym.sym == SDLK_0) {
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                SDL_Color pixel = getPixel(x + szerokosc / 2, y);
                                setPixel(x, y, pixel.r, pixel.g, pixel.b);
                                setPixel(x + szerokosc / 2, y, 0, 0, 0);
                            }
                        }
                        SDL_UpdateWindowSurface(window);
                        displayMainMenu();
                        return;
                    } else {
                        break;
                    }
                }
            }
        }
    }
}

// Dithering
void Funkcja4() {
    if(tryb & 0x40) {
        cout << "\n\nDithering jest dostępny tylko w trybie 16-bit!\n\n";
        displayMainMenu();
        return;
    } else {
        SDL_Event event;
        cout << "\n\n\tWybierz:\n1. Dithering ON\n2. Dithering OFF\n0. Zatwierdź\n";
        while (SDL_WaitEvent(&event)) {
            // sprawdzamy czy pojawiło się zdarzenie
            switch (event.type) {
                // sprawdzamy czy został wciśnięty klawisz
                case SDL_KEYDOWN: {
                    // jeśli Dithering ON
                    if (event.key.keysym.sym == SDLK_1) {
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                SDL_Color pixel = dithering565(x, y);
                                setPixel(x + szerokosc / 2, y, pixel.r, pixel.g, pixel.b);
                            }
                        }
                        if(!(tryb & 0x20)) {
                            tryb += 0b00100000;
                        }
                        SDL_UpdateWindowSurface(window);
                        break;
                    }
                    // jeśli Dithering OFF
                    if (event.key.keysym.sym == SDLK_2) {
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                SDL_Color pixel = getPixel(x, y);
                                setPixel(x + szerokosc / 2, y, pixel.r, pixel.g, pixel.b);
                            }
                        }
                        if(tryb & 0x20) {
                            tryb -= 0b00100000;
                        }
                        SDL_UpdateWindowSurface(window);
                        break;
                    }
                    // jeśli zatwierdź
                    if (event.key.keysym.sym == SDLK_0) {
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                SDL_Color pixel = getPixel(x + szerokosc / 2, y);
                                setPixel(x, y, pixel.r, pixel.g, pixel.b);
                                setPixel(x + szerokosc / 2, y, 0, 0, 0);
                            }
                        }
                        SDL_UpdateWindowSurface(window);
                        displayMainMenu();
                        return;
                    } else {
                        break;
                    }
                }
            }
        }
    }
}

// Predykcja
void Funkcja5() {
    SDL_Event event;
    cout << "\n\n\tWybierz:\n1. Filtr różnicowy ON\n2. Filtr różnicowy OFF\n0. Zatwierdź\n";
    vector<Uint8> Rr;
    vector<Uint8> Gr;
    vector<Uint8> Br;
    while (SDL_WaitEvent(&event)) {
        // sprawdzamy czy pojawiło się zdarzenie
        switch (event.type) {
            // sprawdzamy czy został wciśnięty klawisz
            case SDL_KEYDOWN: {
                // jeśli Filtr różnicowy ON
                if (event.key.keysym.sym == SDLK_1) {
                    if(tryb & 0x40) {
                        if(tryb & 0x08) {
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
                            Rr = reverseFiltrRoznicowy(Rp);
                            Gr = reverseFiltrRoznicowy(Gp);
                            Br = reverseFiltrRoznicowy(Bp);
                            int i = 0;
                            for(int x = 0; x < szerokosc / 2; x++) {
                                for(int y = 0; y < wysokosc / 2; y++) {
                                    SDL_Color pixel = HSLtoRGB(Rr[i], Gr[i], Br[i]);
                                    setPixel(x + szerokosc / 2, y, pixel.r, pixel.g, pixel.b);
                                    i++;
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
                            Rr = reverseFiltrRoznicowy(Rp);
                            Gr = reverseFiltrRoznicowy(Gp);
                            Br = reverseFiltrRoznicowy(Bp);
                            int i = 0;
                            for(int x = 0; x < szerokosc / 2; x++) {
                                for(int y = 0; y < wysokosc / 2; y++) {
                                    setPixel(x + szerokosc / 2, y, Rr[i], Gr[i], Br[i]);
                                    i++;
                                }
                            }
                            R.clear();
                            G.clear();
                            B.clear();
                            Rp.clear();
                            Gp.clear();
                            Bp.clear();
                        }
                    } else {
                        vector<Uint16> obraz;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                obraz.push_back(getRGB565(x, y));
                            }
                        }
                        vector<Uint16> obrazP = filtrRoznicowy(obraz);
                        vector<Uint16> obrazR = reverseFiltrRoznicowy(obrazP);
                        int i = 0;
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                setRGB565(x + szerokosc / 2, y, obrazR[i]);
                                i++;
                            }
                        }
                        obraz.clear();
                        obrazP.clear();
                        obrazR.clear();
                    }

                    if(!(tryb & 0x10)) {
                        tryb += 0b00010000;
                    }
                    SDL_UpdateWindowSurface(window);
                    break;
                }
                // jeśli Filtr różnicowy OFF
                if (event.key.keysym.sym == SDLK_2) {
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            SDL_Color pixel = getPixel(x, y);
                            setPixel(x + szerokosc / 2, y, pixel.r, pixel.g, pixel.b);
                        }
                    }
                    if(tryb & 0x10) {
                        tryb -= 0b00010000;
                    }
                    SDL_UpdateWindowSurface(window);
                    break;
                }
                // jeśli zatwierdź
                if (event.key.keysym.sym == SDLK_0) {
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            SDL_Color pixel = getPixel(x + szerokosc / 2, y);
                            setPixel(x, y, pixel.r, pixel.g, pixel.b);
                            setPixel(x + szerokosc / 2, y, 0, 0, 0);
                        }
                    }
                    SDL_UpdateWindowSurface(window);
                    displayMainMenu();
                    Rr.clear();
                    Gr.clear();
                    Br.clear();
                    return;
                } else {
                    break;
                }
            }
        }
    }
}

// Kompresja bezstratna LZ77
void Funkcja6() {
    SDL_Event event;
    bool processed = false;
    cout << "\n\n\tWybierz:\n1. LZ77 ON\n2. LZ77 OFF\n0. Zatwierdź\n";
    vector<Uint8> Hh;
    vector<Uint8> Ss;
    vector<Uint8> Ll;
    vector<Uint8> Rr;
    vector<Uint8> Gr;
    vector<Uint8> Br;
    vector<Uint16> obrazR;
    while (SDL_WaitEvent(&event)) {
        // sprawdzamy czy pojawiło się zdarzenie
        switch (event.type) {
            // sprawdzamy czy został wciśnięty klawisz
            case SDL_KEYDOWN: {
                // jeśli LZ77 ON
                if (event.key.keysym.sym == SDLK_1) {
                    if (tryb & 0x40) {
                        if(tryb & 0x08) {
                            if (!processed) {
                                HSL hsl;
                                vector<Uint8> H;
                                vector<Uint8> S;
                                vector<Uint8> L;
                                for (int x = 0; x < szerokosc / 2; x++) {
                                    for (int y = 0; y < wysokosc / 2; y++) {
                                        hsl = RGBtoHSL(x, y);
                                        H.push_back(hsl.H);
                                        S.push_back(hsl.S);
                                        L.push_back(hsl.L);
                                    }
                                }
                                H.insert(H.end(), S.begin(), S.end());
                                H.insert(H.end(), L.begin(), L.end());
                                LZ77output8 = LZ77Kompresja(H, H.size());
                                vector<Uint8> combined = LZ77Dekompresja(LZ77output8);
                                for(int i = 0; i < 64000; i++) {
                                    Hh.push_back(combined[i]);
                                    Ss.push_back(combined[i + 64000]);
                                    Ll.push_back(combined[i + 128000]);
                                }
                                combined.clear();
                                H.clear();
                                S.clear();
                                L.clear();
                                processed = true;
                            }
                            int i = 0;
                            for (int x = 0; x < szerokosc / 2; x++) {
                                for (int y = 0; y < wysokosc / 2; y++) {
                                    SDL_Color pixel = HSLtoRGB(Hh[i], Ss[i], Ll[i]);
                                    setPixel(x + szerokosc / 2, y, pixel.r, pixel.g, pixel.b);
                                    i++;
                                }
                            }
                        } else {
                            if(!processed) {
                                vector<Uint8> R;
                                vector<Uint8> G;
                                vector<Uint8> B;
                                for (int x = 0; x < szerokosc / 2; x++) {
                                    for (int y = 0; y < wysokosc / 2; y++) {
                                        SDL_Color pixel = getPixel(x, y);
                                        R.push_back(pixel.r);
                                        G.push_back(pixel.g);
                                        B.push_back(pixel.b);
                                    }
                                }
                                R.insert(R.end(), G.begin(), G.end());
                                R.insert(R.end(), B.begin(), B.end());
                                LZ77output8 = LZ77Kompresja(R, R.size());
                                vector<Uint8> combined = LZ77Dekompresja(LZ77output8);
                                for(int i = 0; i < 64000; i++) {
                                    Rr.push_back(combined[i]);
                                    Gr.push_back(combined[i + 64000]);
                                    Br.push_back(combined[i + 128000]);
                                }
                                combined.clear();
                                R.clear();
                                G.clear();
                                B.clear();
                                processed = true;
                            }
                            int i = 0;
                            for (int x = 0; x < szerokosc / 2; x++) {
                                for (int y = 0; y < wysokosc / 2; y++) {
                                    setPixel(x + szerokosc / 2, y, Rr[i], Gr[i], Br[i]);
                                    i++;
                                }
                            }
                        }
                    } else {
                        if(!processed) {
                            vector<Uint16> obraz;
                            for (int x = 0; x < szerokosc / 2; x++) {
                                for (int y = 0; y < wysokosc / 2; y++) {
                                    obraz.push_back(getRGB565(x, y));
                                }
                            }
                            LZ77output16 = LZ77Kompresja(obraz, obraz.size());
                            obraz.clear();
                            obrazR = LZ77Dekompresja(LZ77output16);
                            processed = true;
                        }
                        int i = 0;
                        for (int x = 0; x < szerokosc / 2; x++) {
                            for (int y = 0; y < wysokosc / 2; y++) {
                                setRGB565(x + szerokosc / 2, y, obrazR[i]);
                                i++;
                            }
                        }
                    }
                    if (!(tryb & 0x01)) {
                        tryb += 0b00000001;
                    }
                    SDL_UpdateWindowSurface(window);
                    break;
                }
                // jeśli LZ77 OFF
                if (event.key.keysym.sym == SDLK_2) {
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            SDL_Color pixel = getPixel(x, y);
                            setPixel(x + szerokosc / 2, y, pixel.r, pixel.g, pixel.b);
                        }
                    }
                    if(tryb & 0x01) {
                        tryb -= 0b00000001;
                    }
                    SDL_UpdateWindowSurface(window);
                    break;
                }
                // jeśli zatwierdź
                if (event.key.keysym.sym == SDLK_0) {
                    for(int x = 0; x < szerokosc / 2; x++) {
                        for(int y = 0; y < wysokosc / 2; y++) {
                            SDL_Color pixel = getPixel(x + szerokosc / 2, y);
                            setPixel(x, y, pixel.r, pixel.g, pixel.b);
                            setPixel(x + szerokosc / 2, y, 0, 0, 0);
                        }
                    }
                    SDL_UpdateWindowSurface(window);
                    Hh.clear();
                    Ss.clear();
                    Ll.clear();
                    Rr.clear();
                    Gr.clear();
                    Br.clear();
                    obrazR.clear();
                    displayMainMenu();
                    return;
                } else {
                    break;
                }
            }
        }
    }
}

// Kompresja stratna podpróbkowanie 420
void Funkcja7() {
    if(!(tryb & 0x40) || !(tryb & 0x08)) {
        cout << "\n\nPodpróbkowanie jest dostępne tylko w trybie 24-bit HSL!\n\n";
        displayMainMenu();
        return;
    } else if(tryb & 0x02) {
        cout << "\n\nUżycie LZ77 po wykonaniu transformaty DCT jest niemożliwe!\n\n";
        displayMainMenu();
        return;
    } else {
        SDL_Event event;
        cout << "\n\n\tWybierz:"
                "\n1. Podpróbkowanie 420 składowej barwy S"
                "\n2. Podpróbkowanie 420 składowej nasycenia L"
                "\n3. Podpróbkowanie 420 składowych barwy i nasycenia SL"
                "\n4. Podpróbkowanie 420 OFF"
                "\n0. Zatwierdź\n";
        while (SDL_WaitEvent(&event)) {
            // sprawdzamy czy pojawiło się zdarzenie
            switch (event.type) {
                // sprawdzamy czy został wciśnięty klawisz
                case SDL_KEYDOWN: {
                    // jeśli Podpróbkowanie 420 (H) ON
                    if (event.key.keysym.sym == SDLK_1) {
                        for(int x = 0; x < szerokosc / 2; x = x + 2) {
                            for(int y = 0; y < wysokosc / 2; y = y + 2) {
                                pixele4HSL packet;
                                packet.a = RGBtoHSL(x, y);
                                packet.b = RGBtoHSL(x + 1, y);
                                packet.c = RGBtoHSL(x, y + 1);
                                packet.d = RGBtoHSL(x + 1, y + 1);
                                pixele4HSL outputPacket = HSL_420(packet, 's');
                                SDL_Color a = HSLtoRGB(outputPacket.a.H, outputPacket.a.S, outputPacket.a.L);
                                SDL_Color b = HSLtoRGB(outputPacket.b.H, outputPacket.b.S, outputPacket.b.L);
                                SDL_Color c = HSLtoRGB(outputPacket.c.H, outputPacket.c.S, outputPacket.c.L);
                                SDL_Color d = HSLtoRGB(outputPacket.d.H, outputPacket.d.S, outputPacket.d.L);
                                setPixel(x + szerokosc / 2, y, a.r, a.g, a.b);
                                setPixel(x + 1 + szerokosc / 2, y, b.r, b.g, b.b);
                                setPixel(x + szerokosc / 2, y + 1, c.r, c.g, c.b);
                                setPixel(x + 1 + szerokosc / 2, y + 1, d.r, d.g, d.b);
                            }
                        }
                        if(!(tryb & 0x04)) {
                            tryb += 0b00000100;
                        }
                        SDL_UpdateWindowSurface(window);
                        break;
                    }
                    // jeśli Podpróbkowanie 420 (S) ON
                    if (event.key.keysym.sym == SDLK_2) {
                        for(int x = 0; x < szerokosc / 2; x = x + 2) {
                            for(int y = 0; y < wysokosc / 2; y = y + 2) {
                                pixele4HSL packet;
                                packet.a = RGBtoHSL(x, y);
                                packet.b = RGBtoHSL(x + 1, y);
                                packet.c = RGBtoHSL(x, y + 1);
                                packet.d = RGBtoHSL(x + 1, y + 1);
                                pixele4HSL outputPacket = HSL_420(packet, 'l');
                                SDL_Color a = HSLtoRGB(outputPacket.a.H, outputPacket.a.S, outputPacket.a.L);
                                SDL_Color b = HSLtoRGB(outputPacket.b.H, outputPacket.b.S, outputPacket.b.L);
                                SDL_Color c = HSLtoRGB(outputPacket.c.H, outputPacket.c.S, outputPacket.c.L);
                                SDL_Color d = HSLtoRGB(outputPacket.d.H, outputPacket.d.S, outputPacket.d.L);
                                setPixel(x + szerokosc / 2, y, a.r, a.g, a.b);
                                setPixel(x + 1 + szerokosc / 2, y, b.r, b.g, b.b);
                                setPixel(x + szerokosc / 2, y + 1, c.r, c.g, c.b);
                                setPixel(x + 1 + szerokosc / 2, y + 1, d.r, d.g, d.b);
                            }
                        }
                        if(!(tryb & 0x04)) {
                            tryb += 0b00000100;
                        }
                        SDL_UpdateWindowSurface(window);
                        break;
                    }
                    // jeśli Podpróbkowanie 420 (HS) ON
                    if (event.key.keysym.sym == SDLK_3) {
                        for(int x = 0; x < szerokosc / 2; x = x + 2) {
                            for(int y = 0; y < wysokosc / 2; y = y + 2) {
                                pixele4HSL packet;
                                packet.a = RGBtoHSL(x, y);
                                packet.b = RGBtoHSL(x + 1, y);
                                packet.c = RGBtoHSL(x, y + 1);
                                packet.d = RGBtoHSL(x + 1, y + 1);
                                pixele4HSL outputPacket = HSL_420(packet, 's');
                                packet = outputPacket;
                                outputPacket = HSL_420(packet, 'l');
                                SDL_Color a = HSLtoRGB(outputPacket.a.H, outputPacket.a.S, outputPacket.a.L);
                                SDL_Color b = HSLtoRGB(outputPacket.b.H, outputPacket.b.S, outputPacket.b.L);
                                SDL_Color c = HSLtoRGB(outputPacket.c.H, outputPacket.c.S, outputPacket.c.L);
                                SDL_Color d = HSLtoRGB(outputPacket.d.H, outputPacket.d.S, outputPacket.d.L);
                                setPixel(x + szerokosc / 2, y, a.r, a.g, a.b);
                                setPixel(x + 1 + szerokosc / 2, y, b.r, b.g, b.b);
                                setPixel(x + szerokosc / 2, y + 1, c.r, c.g, c.b);
                                setPixel(x + 1 + szerokosc / 2, y + 1, d.r, d.g, d.b);
                            }
                        }
                        if(!(tryb & 0x04)) {
                            tryb += 0b00000100;
                        }
                        SDL_UpdateWindowSurface(window);
                        break;
                    }
                    // jeśli Podpróbkowanie 420 OFF
                    if (event.key.keysym.sym == SDLK_4) {
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                SDL_Color pixel = getPixel(x, y);
                                setPixel(x + szerokosc / 2, y, pixel.r, pixel.g, pixel.b);
                            }
                        }
                        if(tryb & 0x04) {
                            tryb -= 0b00000100;
                        }
                        SDL_UpdateWindowSurface(window);
                        break;
                    }
                    // jeśli zatwierdź
                    if (event.key.keysym.sym == SDLK_0) {
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                SDL_Color pixel = getPixel(x + szerokosc / 2, y);
                                setPixel(x, y, pixel.r, pixel.g, pixel.b);
                                setPixel(x + szerokosc / 2, y, 0, 0, 0);
                            }
                        }
                        SDL_UpdateWindowSurface(window);
                        displayMainMenu();
                        return;
                    } else {
                        break;
                    }
                }
            }
        }
    }
}

// DCT
void Funkcja8() {
    if(!(tryb & 0x40)) {
        cout << "\n\nDCT jest dostępne tylko w trybie 24-bit!\n\n";
        displayMainMenu();
        return;
    } else {
        bool processed;
        vector<macierz> outputAllBlocks;
        SDL_Event event;
        cout << "\n\n\tWybierz:"
                "\n1. DCT ON"
                "\n2. DCT OFF"
                "\n0. Zatwierdź\n";
        while (SDL_WaitEvent(&event)) {
            // sprawdzamy czy pojawiło się zdarzenie
            switch (event.type) {
                // sprawdzamy czy został wciśnięty klawisz
                case SDL_KEYDOWN: {
                    // jeśli DCT ON
                    if (event.key.keysym.sym == SDLK_1) {
                        if(!processed) {
                            dctOutput = DCTKompresja(tryb);
                            outputAllBlocks = DCTDekompresja(dctOutput, tryb);
                            processed = true;
                        }
                        // wyjscie danych na ekran
                        if(tryb & 0x80) /* Color */ {
                            if(tryb & 0x08) /* HSL */ {
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
                                                setPixel(x + (l * rozmiarBloku) + szerokosc / 2,
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
                                                setPixel(x + (l * rozmiarBloku) + szerokosc / 2,
                                                         y + (k * rozmiarBloku),
                                                         outputAllBlocks[(k * 40) + l].dane[x][y],
                                                         outputAllBlocks[(k * 40) + l + 1000].dane[x][y],
                                                         outputAllBlocks[(k * 40) + l + 2000].dane[x][y]);
                                            }
                                        }
                                    }
                                }
                            }
                        } else /* NO COLOR */ {
                            // po wysokosci -> 25 przebiegow
                            for(int k = 0; k < (wysokosc / 2 / rozmiarBloku); k++) {
                                // po szerokosci -> 40 przebiegow
                                for (int l = 0; l < (szerokosc / 2 / rozmiarBloku); l++) {
                                    // pobranie danych w bloku
                                    for (int x = 0; x < rozmiarBloku; x++) {
                                        for (int y = 0; y < rozmiarBloku; y++) {
                                            setPixel(x + (l * rozmiarBloku) + szerokosc / 2,
                                                     y + (k * rozmiarBloku),
                                                     outputAllBlocks[(k * 40) + l].dane[x][y],
                                                     outputAllBlocks[(k * 40) + l].dane[x][y],
                                                     outputAllBlocks[(k * 40) + l].dane[x][y]);
                                        }
                                    }
                                }
                            }
                        }
                        if(!(tryb & 0x02)) {
                            tryb += 0b00000010;
                        }
                        SDL_UpdateWindowSurface(window);
                        break;
                    }
                    // jeśli DCT OFF
                    if (event.key.keysym.sym == SDLK_2) {
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                SDL_Color pixel = getPixel(x, y);
                                setPixel(x + szerokosc / 2, y, pixel.r, pixel.g, pixel.b);
                            }
                        }
                        if(tryb & 0x02) {
                            tryb -= 0b00000010;
                        }
                        SDL_UpdateWindowSurface(window);
                        break;
                    }
                    // jeśli zatwierdź
                    if (event.key.keysym.sym == SDLK_0) {
                        for(int x = 0; x < szerokosc / 2; x++) {
                            for(int y = 0; y < wysokosc / 2; y++) {
                                SDL_Color pixel = getPixel(x + szerokosc / 2, y);
                                setPixel(x, y, pixel.r, pixel.g, pixel.b);
                                setPixel(x + szerokosc / 2, y, 0, 0, 0);
                            }
                        }
                        SDL_UpdateWindowSurface(window);
                        displayMainMenu();
                        outputAllBlocks.clear();
                        return;
                    } else {
                        break;
                    }
                }
            }
        }
    }
}

// zapis do pliku
void Funkcja9() {
    string filename;
    cout << "\n\tPodaj nazwę pliku do zapisu (bez rozszerzenia):" << endl;
    cin >> filename;
    ZapiszPlik(filename, tryb);
    displayMainMenu();
    SDL_UpdateWindowSurface(window);
}

// odczyt z pliku
void Funkcja10() {
    string filename;
    cout << "\n\tPodaj nazwę pliku do odczytu (bez rozszerzenia):" << endl;
    cin >> filename;
    OdczytajPlik(filename);
    displayMainMenu();
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

