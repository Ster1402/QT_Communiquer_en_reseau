#include "client.h"
#include "fenetre.h"

void FenetreChat::dessinClient()
{
    ongletClient = new QWidget;
    //Nous sommes par défaut client
    client = new Client();
    QVBoxLayout *layout = new QVBoxLayout;

        QLabel *labPseudo = new QLabel("<strong>Pseudo : </strong>");
        QLabel *labMessage = new QLabel("<strong>Message : </strong>");

        QGridLayout *grille = new QGridLayout;
        grille->addWidget (labPseudo,0,0,1,1);
        grille->addWidget (pseudo,0,1,1,3);
        grille->addWidget (labMessage,1,0,1,1);
        grille->addWidget (message,1,1,1,3);

        layout->addLayout (grille);
        layout->addSpacing (15);
        layout->addWidget (envoyer);
        layout->addWidget (listeMessages);

        ongletClient->setLayout (layout );

        connect (client->getSocketClient (),SIGNAL(readyRead()),this,SLOT(donneesRecuesClient ()));
        connect (client->getSocketClient (),SIGNAL(connected()),this,SLOT(connecte ()));
        connect (client->getSocketClient (),SIGNAL (disconnected()),this,SLOT(deconnecte ()));
        connect (client->getSocketClient (),SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(erreurSocket(QAbstractSocket::SocketError)));
        //Connexions côté client

        connect (envoyer,SIGNAL (clicked()),this,SLOT(boutonEnvoyer()));
        connect (message,SIGNAL (returnPressed()),this,SLOT(onMessageR()));

        client->setTailleMessage (0);

        indClient = 0;
        indClient = onglets->addTab (ongletClient,"Discussion");

}

void FenetreChat::onMessageR ()
{
    boutonEnvoyer ();
}

void FenetreChat::boutonEnvoyer ()
{
    QByteArray paquet;
    QDataStream out(&paquet,QIODevice::WriteOnly);

    //Préparation du message à envoyer
    QString messageAEnvoyer = "<strong>"+pseudo->text ()+"</strong> : "+message->text ();

    out<<(quint16)0;
    out<<messageAEnvoyer;
    out.device ()->seek (0);
    out<< (quint16)(paquet.size () - sizeof (quint16));

    client->getSocketClient ()->write (paquet); //On envoie le paquet

    message->clear ();
    message->setFocus ();
}

void FenetreChat::donneesRecuesClient ()
{
    QDataStream in(client->getSocketClient ());
    quint16 taille = 0;

       if(client->getTailleMessage () == 0)
    {
        if(client->getSocketClient ()->bytesAvailable () < (int)sizeof (quint16))
            return;


        in >> taille;
        client->setTailleMessage (taille);
    }

    if(client->getSocketClient ()->bytesAvailable () < client->getTailleMessage ())
        return;

    QString msg;
    in >> msg;

    listeMessages->append (msg);
    client->setTailleMessage (0);
}

void FenetreChat::connecte ()
{
    listeMessages->append ("<strong>Connexion réussit ! </strong><br>");
    connexion->setEnabled (true);
    devientVert ();
}

void FenetreChat::deconnecte ()
{
    listeMessages->append ("<strong>Oups...Vous êtes déconnecté du serveur ! </strong><br>");
    devientRouge ();
}

void FenetreChat::erreurSocket (QAbstractSocket::SocketError erreur)
{
        switch(erreur) // On affiche un message différent selon l'erreur qu'on nous indique
        {
            case QAbstractSocket::HostNotFoundError:
                listeMessages->append(tr("<em>ERREUR : le serveur n'a pas pu être trouvé. Vérifiez l'IP et le port.</em>"));
                break;
            case QAbstractSocket::ConnectionRefusedError:
                listeMessages->append(tr("<em>ERREUR : le serveur a refusé la connexion. Vérifiez si le programme \"serveur\" a bien été lancé. Vérifiez aussi l'IP et le port.</em>"));
                break;
            case QAbstractSocket::RemoteHostClosedError:
                listeMessages->append(tr("<em>ERREUR : le serveur a coupé la connexion.</em>"));
                break;
            default:
                listeMessages->append(tr("<em>ERREUR : ") + client->getSocketClient ()->errorString() + tr("</em>"));
        }

    connexion->setEnabled (true);
}


void FenetreChat::seDeconnect ()
{
    //On supprime les possibles connexions précédente
    client->getSocketClient ()->abort ();

}
