#include "client.h"

Client::Client()
{
    socketClient = new QTcpSocket();
    pseudo = "Un client";
    tailleMessage = 0;

}

//Accesseurs
QTcpSocket *Client::getSocketClient() { return socketClient; }
QString Client::getPseudo(){ return pseudo; }
quint16 Client::getTailleMessage() { return tailleMessage; }



//Mutateurs
void Client::setPseudo(QString pseudo){
    this->pseudo = pseudo;
}

void Client::setSocketClient(QTcpSocket * socket){
    socketClient = socket;
}

void Client::setTailleMessage(quint16 taille){
    tailleMessage = taille;
}
