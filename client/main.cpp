#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <random>
#include <string>
#include <condition_variable>
#include "my_socket.h"
#include "Hrac.h"
#include <sstream>


struct hodKockou {
    int cislo;
    static hodKockou generate();
};

hodKockou hodKockou::generate() {
    int cislo = rand() % 6 + 1;
    hodKockou point = {cislo};
    return point;
}

class ThreadData {
public:
    ThreadData(int bufferCapacity, MySocket* serverSocket);
    void produce();
    hodKockou consume();
private:
    const int bufferCapacity;
    std::queue<hodKockou> buffer;
    std::mutex mutex;
    std::condition_variable isFull;
    std::condition_variable isEmpty;
    MySocket* serverSocket;
};

ThreadData::ThreadData(int bufferCapacity, MySocket* serverSocket) :
        bufferCapacity(bufferCapacity),
        buffer(),
        mutex(),
        isFull(),
        isEmpty(),
        serverSocket(serverSocket) {

}

void ThreadData::produce() {
    for (int i = 0; i < 1000; ++i) {
        hodKockou item = hodKockou::generate();
        {
            std::unique_lock<std::mutex> lock(this->mutex);
            while (static_cast<int>(this->buffer.size()) >= bufferCapacity) {
                this->isEmpty.wait(lock);
            }
            this->buffer.push(item);
            this->isFull.notify_one();
        }
    }
}

hodKockou ThreadData::consume() {
    hodKockou item;
    {
        std::unique_lock<std::mutex> lock(this->mutex);
        while (this->buffer.size() <= 0) {
            this->isFull.wait(lock);
        }
        item = this->buffer.front();
        this->buffer.pop();
        this->isEmpty.notify_one();
    }
    return item;
}

void produce(ThreadData& data) {
    data.produce();
}

std::vector<std::string> spracujSpravuZoServera(const std::string& basicString) {
    std::vector<std::string> vysledok;
    std::stringstream ss(basicString);
    std::string polozka;

    while (std::getline(ss, polozka, ';')) {
        vysledok.push_back(polozka);
    }

    return vysledok;
}

void spracuj(const std::string& basicString, Hrac* hrac, ThreadData* data) {
    std::vector<std::string> spracovanaSprava = spracujSpravuZoServera(basicString);
    if (spracovanaSprava[0] == "hernaPlocha") {
        //std::cout << std::string(25, '\n');
        std::cout << "Pocet hracov: " << spracovanaSprava[1] << std::endl;
        for (int i = 0; i < std::stoi(spracovanaSprava[1]); ++i) {
            std::string farba;
            int pocetFiguriekVZaciatocnom;
            if (i == 0) {
                farba = "Modreho";
                pocetFiguriekVZaciatocnom = std::stoi(spracovanaSprava[2]);
            } else if (i == 1) {
                farba = "Cerveneho";
                pocetFiguriekVZaciatocnom = std::stoi(spracovanaSprava[3]);
            } else if (i == 2) {
                farba = "Zeleneho";
                pocetFiguriekVZaciatocnom = std::stoi(spracovanaSprava[4]);
            } else {
                farba = "Oranzoveho";
                pocetFiguriekVZaciatocnom = std::stoi(spracovanaSprava[5]);
            }
            std::cout << "Pocet figuriek " << farba << " hraca v zaciatocnom domceku: " << pocetFiguriekVZaciatocnom << std::endl;
        }
        std::string hernaPlocha;
        for (int i = (std::stoi(spracovanaSprava[1]) * 2) + 2; i < spracovanaSprava.size(); ++i) {
            hernaPlocha += spracovanaSprava[i];
        }
        std::vector<std::vector<char>> dvojrozmernePole(15, std::vector<char>(15, ' '));

        int index = 0;

        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                if (((i < 6 || i > 8) && (j == 6 || j == 8)) || ((j < 6 || j > 8) && (i == 6 || i == 8)) ||
                    (i == 0 && j == 7) || (i == 14 && j == 7) || (i == 7 && j == 0) || (i == 7 && j == 14)) {
                    if (index < hernaPlocha.size() && hernaPlocha[index] == 'M') {
                        dvojrozmernePole[i][j] = 'M';
                    } else if (index < hernaPlocha.size() && hernaPlocha[index] == 'C') {
                        dvojrozmernePole[i][j] = 'C';
                    } else if (index < hernaPlocha.size() && hernaPlocha[index] == 'Z') {
                        dvojrozmernePole[i][j] = 'Z';
                    } else if (index < hernaPlocha.size() && hernaPlocha[index] == 'O') {
                        dvojrozmernePole[i][j] = 'O';
                    } else {
                        dvojrozmernePole[i][j] = '*';
                    }
                    index++;
                }
            }
        }

        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                std::cout << dvojrozmernePole[i][j] << " ";
            }
            std::cout << std::endl;
        }
        for (int i = 0; i < std::stoi(spracovanaSprava[1]); ++i) {
            int pocetFiguriekVKoncovom;
            std::string farba;
            if (i == 0) {
                farba = "Modreho";
                pocetFiguriekVKoncovom = std::stoi((spracovanaSprava[3]));
            } else if (i == 1) {
                farba = "Cerveneho";
                pocetFiguriekVKoncovom = std::stoi((spracovanaSprava[5]));
            } else if (i == 2) {
                farba = "Zeleneho";
                pocetFiguriekVKoncovom = std::stoi((spracovanaSprava[7]));
            } else {
                farba = "Oranzoveho";
                pocetFiguriekVKoncovom = std::stoi((spracovanaSprava[9]));
            }
            std::cout << "Pocet figuriek " << farba << " hraca v koncovom domceku: " << pocetFiguriekVKoncovom << std::endl;
        }

    } else if (spracovanaSprava[0] == "Hra sa zacne za ...\n" || spracovanaSprava[0] == "3\n" || spracovanaSprava[0] == "2\n" || spracovanaSprava[0] == "1\n") {
        std::cout << spracovanaSprava[0];
    } else if (spracovanaSprava[0] == "poradieHraca") {
        std::string farba;
        if (spracovanaSprava[1] == "1") {
            farba = "Modra 'M'";
        } else if (spracovanaSprava[1] == "2") {
            farba = "Cervena 'C'";
        } else if (spracovanaSprava[1] == "3") {
            farba = "Zelena 'Z'";
        } else {
            farba = "Oranzova 'O'";
        }

        std::cout << "Na tahu je hrac cislo " << std::stoi(spracovanaSprava[1]) << " s farbou " << farba << std::endl;
        if (hrac->getIdHraca() == std::stoi(spracovanaSprava[1])) {
            char tlacidlo;
            std::cout << "Stlac 'e' aby si hodil kockou.";
            while (tlacidlo != 'e') {
                std::cin >> tlacidlo;
            }
            hodKockou hod = data->consume();
            std::cout << "Hodil si cislo: " << hod.cislo << std::endl;
        }
    } else if (spracovanaSprava[0] == "je pripravenych") {
        std::cout << spracovanaSprava[0] << spracovanaSprava[1] << std::endl;
    }
}

int main() {
    srand(time(0));
    //printf("Hello");
    MySocket* mySocket = MySocket::createConnection("frios2.fri.uniza.sk", 15874);

    std::string sprava = mySocket->prijmi();

    std::vector<std::string> oddeleneSpravy = spracujSpravuZoServera(sprava);

    std::string farbaFigurky;

    if (oddeleneSpravy[1] == "M") {
        farbaFigurky = "Modra 'M'";
    } else if (oddeleneSpravy[1] == "C") {
        farbaFigurky = "Cervena 'C'";
    } else if (oddeleneSpravy[1] == "Z") {
        farbaFigurky = "Zelena 'Z'";
    } else {
        farbaFigurky = "Oranzova 'O'";
    }


    std::cout << "Si hrac cislo " << oddeleneSpravy[0] << " s farbou figurky " << farbaFigurky << std::endl;

    char farba = oddeleneSpravy[1][0];

    int cisloHraca = stoi(sprava);


    Hrac* hrac = new Hrac(cisloHraca, farba);

    std::cout << "Ak si pripraveny stlac 'r'." << std::endl;

    char ready;

    while(ready != 'r') {
        std::cin >> ready;
    }

    hrac->jePripraveny();

    std::string ohlasServer;

    ohlasServer = "Hrac " + oddeleneSpravy[0] + " je pripraveny";

    mySocket->sendData(ohlasServer);
    ThreadData data(100, mySocket);
    std::thread thProduce(produce, std::ref(data));
    while(true) {
        std::string response =  mySocket->prijmi();
        //std::cout << response;
        spracuj(response, hrac, &data);
    }

    mySocket->sendEndMessage();

    thProduce.join();
    delete hrac;
    hrac = nullptr;
    delete mySocket;
    mySocket = nullptr;
    return 0;
}
