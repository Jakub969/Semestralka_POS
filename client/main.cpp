#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <random>
#include <string>
#include <condition_variable>
#include "my_socket.h"
#include "Hrac.h"
#include "HernaPlocha.h"
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

    MySocket *getServerSocket() const;

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

MySocket *ThreadData::getServerSocket() const {
    return serverSocket;
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
    // hernaPlocha;pocetHracov;hracNaRade;aktualnePozicieFigurky...;
    if (spracovanaSprava[0] == "hernaPlocha") {
        //std::cout << std::string(25, '\n');
        HernaPlocha hernaPlocha(spracovanaSprava);
        hernaPlocha.vypisSa();

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
    ThreadData data(10, mySocket);
    std::thread thProduce(produce, std::ref(data));
    while(true) {
        std::string response =  mySocket->prijmi();
        std::cout << response;
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
