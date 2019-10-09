#ifndef CLIENT_H
#define CLIENT_H

#include <QtWidgets>
#include <QtNetwork>


class Client
{

public:

    Client();

    //Accesseurs Mutateurs
    QTcpSocket* getSocketClient();
    QString getPseudo();
    quint16 getTailleMessage();


    void setSocketClient(QTcpSocket*);
    void setPseudo(QString);
    void setTailleMessage(quint16);

private:
    QTcpSocket* socketClient;
    QString pseudo;
    quint16 tailleMessage;

};

#endif // CLIENT_H
