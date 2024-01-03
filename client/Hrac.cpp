//
// Created by jakub on 2. 1. 2024.
//

#include <iostream>
#include "Hrac.h"


Hrac::Hrac(int idHraca, char farbaFigurky) {
    this->idHraca = idHraca;
    this->farbaFigurky = farbaFigurky;
    this->jeReady = false;
}

void Hrac::jePripraveny() {
    this->jeReady = true;
    std::cout << "Hrac " << idHraca << " je pripraveny!" << std::endl;
}
