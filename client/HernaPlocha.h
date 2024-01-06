//
// Created by jakub on 5. 1. 2024.
//

#ifndef CLIENT_HERNAPLOCHA_H
#define CLIENT_HERNAPLOCHA_H


#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>

class Suradnica {
public:
    int r;
    int s;
    Suradnica(int r, int s);
    Suradnica() {};
};


class HernaPlocha {
private:
    char** pole;
    const int velkostMapy = 11;
    int pocetHracov;
    int hracNaRade;
public:
    HernaPlocha(std::vector<std::string> &parametre);
    ~HernaPlocha();
    void vypisSa();
    void aktualizujSa(std::vector<std::string> &parametre);
private:
    void alokujPole();
    std::string getFarbuHracaNaRade();
    Suradnica getSuradnicuPodlaIndexu(int index);
    void nastavDefaultnePolicka();
    std::vector<Suradnica> getSlovnik();
    char getZnakPodlaCislaHraca(int cisloHraca);
    std::vector<Suradnica> getZapornySlovnik();
};


#endif //CLIENT_HERNAPLOCHA_H
