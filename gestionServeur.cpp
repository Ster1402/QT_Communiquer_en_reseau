#include "fenetre.h"
//couleur connexion
void couleurVert(QLabel *label)
{
    QColor couleur(Qt::darkGreen);
    QPalette palette;
    palette.setColor (QPalette::WindowText,couleur);
    label->setPalette (palette);
}

void couleurRouge(QLabel *label)
{
    QColor couleur(Qt::darkRed);
    QPalette palette;
    palette.setColor (QPalette::WindowText,couleur);
    label->setPalette (palette);

}

void couleurOr(QListView *label)
{
    QColor couleur(Qt::yellow);
    QPalette palette;
    palette.setColor (QPalette::Base,couleur);
    label->setPalette (palette);

}

void FenetreChat::dessinServeur()
{
    ongletServeur = new QWidget;
    indexServ = onglets->addTab (ongletServeur,"Serveur");

    QVBoxLayout *layout = new QVBoxLayout;

        etatServeur = new QLabel("Serveur inactif");
        etatServeur->setFrameShape (QFrame::Box);
        etatServeur->setFrameShadow (QFrame::Sunken);
        etatServeur->setLineWidth (3);
        etatServeur->setAlignment (Qt::AlignCenter);
        couleurVert (etatServeur);

        port = new QLabel("Port de communication : (Actuellement indisponible...)");
        port->setAlignment (Qt::AlignCenter);

    QLabel *labUtilisateur = new QLabel("Liste des utilisateurs connectés");
        labUtilisateur->setFrameShape (QFrame::Box);
        labUtilisateur->setFrameShadow (QFrame::Sunken);
        labUtilisateur->setLineWidth (2);
        labUtilisateur->setAlignment (Qt::AlignCenter);
        couleurRouge (labUtilisateur);

    vue = new QListView;

    modele = new QStringListModel(listeEtatClient);
    vue->setModel (modele);
    vue->setSelectionMode (QAbstractItemView::NoSelection);
    vue->setEnabled (false);

    couleurOr (vue);

    //...
    gestionListeUser();

    layout->addWidget (etatServeur);
    layout->addWidget (port);
    layout->addWidget (labUtilisateur);
    layout->addWidget (vue);

    ongletServeur->setLayout (layout );
}

void FenetreChat::gestionListeUser()
{
   QStringList listeEtatClient;
   listeEtatClient << ("Il y a ---  "+QString::number  ( listeClients.size () ) + "   --- Client(s) connecté(s) ! ");

   modele->setStringList (listeEtatClient);
   vue->setModel (modele);
}


//On démarre le serveur

void FenetreChat::newServeur ()
{
    //On empêche la création d'un autre serveur
    disconnect (lancerNouveauServeur,SIGNAL (triggered()),this,SLOT(newServeur()));
    onglets->setCurrentIndex (indexServ);
    //Ouverture d'un serveur
    clientServeur = new Client();
    serveur = new QTcpServer(ongletServeur);

    //On lançe un serveur sur notre port
    if(serveur->listen (QHostAddress::Any)){

        //Si on a réussit à demarrer
        etatServeur->setText ("Serveur Actif !");
        port->setText ("Le Serveur à démarrer sur le port : " + QString::number (serveur->serverPort ()));
    }
    else{
        etatServeur->setText ("Le Serveur n'a pas pu être lançé :/");
        port->setText ("Serveur inactif car : " + serveur->errorString ());
    }

    connect (serveur,SIGNAL(newConnection()),this,SLOT(nouvelleConnexion()));

    //On se connecte automatiquement à nôtre serveur
    PortServeur->setValue (serveur->serverPort ());
    IPserveur->setText ("127.0.0.1");
    boutonConnexion ();

    gestionListeUser();
}

//Lorsqu'un nouveau client se connecte

void FenetreChat::nouvelleConnexion ()
{

    envoyerATous ("<strong>" +clientServeur->getPseudo ()+"</strong>" + tr(" est connecté !"));
    clientServeur = new Client();

    //On récupère un pointeur sur l'object Client qui c'est connecter
    clientServeur->setSocketClient (serveur->nextPendingConnection ());

    //On l'ajoute à la liste des personnes connectés
    listeClients << (clientServeur->getSocketClient ());

    //Mise à jour de la vue
    //...
    gestionListeUser();
    //...

    //On informe tous le monde qu'il vient de se connecter


    //connexion
    connect(clientServeur->getSocketClient (), SIGNAL(readyRead()), this, SLOT(donneesRecues()));
    connect(clientServeur->getSocketClient (), SIGNAL(disconnected()), this, SLOT(deconnexionClient()));

}


//On reçoit des données de notre client

void FenetreChat::donneesRecues ()
{
    clientServeur = new Client();
    clientServeur->setSocketClient (qobject_cast<QTcpSocket*> (sender ()));

    if(clientServeur->getSocketClient ()==nullptr)
        return;

    //On crée un flux pour lire ce que contient la socket de notre client
    QDataStream in(clientServeur->getSocketClient ());

    if(clientServeur->getTailleMessage () == 0)
    {
        if(clientServeur->getSocketClient ()->bytesAvailable () < (int)(sizeof(quint16)))
            return;

        quint16 taille = 0;
        in >> taille;
        clientServeur->setTailleMessage (taille);
    }

    //Si on a pas reçu le message en entier
    if (clientServeur->getSocketClient ()->bytesAvailable () < clientServeur->getTailleMessage ())
        return;


    //On peut récupérer le msg si tout est bon
    QString message;
    in >> message;

    //ON DOIT POUR LE PRIVEE CHANGER CETTE METHODE PAR : envoyerA(...)
    envoyerATous (message);

    clientServeur->setTailleMessage (0);

}

void FenetreChat::deconnexionClient ()
{
    clientServeur = new Client();
    clientServeur->setSocketClient (qobject_cast<QTcpSocket*>(sender ()));


    envoyerATous ("<strong>"+clientServeur->getPseudo ()+"</strong> c'est déconnecté !");

    if(clientServeur->getSocketClient ()== nullptr)
        return;

    listeClients.removeOne (clientServeur->getSocketClient());

    //Mise à jour de la vue

    gestionListeUser();
    //...

    (clientServeur->getSocketClient ())->deleteLater ();

}


//Il faudrait implémenté aussi envoyerA(...)

void FenetreChat::envoyerATous(const QString &message)
{
    //Préparation du paquet
    QByteArray paquet;
    QDataStream out(&paquet,QIODevice::WriteOnly);

    out << (quint16)0; //On reserve la plaçe
    out << message; //On met le message dans le paquet
    out.device ()->seek (0); //On se replaçe au début du paquet
    out << (quint16)(paquet.size () - sizeof(quint16)); //On y met la taille du message


    //On envoie le paquet a tout le monde

    for (int i(0); i < listeClients.size () ; i++)
    {
        listeClients[i]->write (paquet);
    }

    clientServeur->setTailleMessage (0);
}



//....
void FenetreChat::closeServeur ()
{
    qApp->quit ();
}
