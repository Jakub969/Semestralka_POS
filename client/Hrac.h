//
// Created by Daniel on 3. 1. 2024.
//

#ifndef CLIENT_HRAC_H
#define CLIENT_HRAC_H


class Hrac {
private:
    int idHraca;
    char farbaFigurky;
    bool jeReady;
public:

    Hrac(int idHraca, char farbaFigurky);


    void jePripraveny();
};


#endif //CLIENT_HRAC_H
