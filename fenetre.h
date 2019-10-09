#ifndef FENETRE_H
#define FENETRE_H

#include "client.h"

void couleurVert(QLabel *label);
void couleurRouge(QLabel *label);
void couleurOr(QListView *label);

class FenetreChat : public QMainWindow
{
    Q_OBJECT

public:
    FenetreChat();
    void creationMenu();
    void creationBarreOutils();
    void creationAction();
    void initialisationVariable();
    void dessinClient();
    void dessinServeur();
    void dessinParametre();
    void connexionSlots();


    //Pour le serveur
    //void envoyerA(QList<Client*>,const QString&); //On peut sélectionner à qui on doit envoyer notre message
    void envoyerATous(const QString&); //On peut envoyer le message à tous le monde, lorsque (All est sélectionner
    void gestionListeUser();

    //La zone d'envoie
    void devientRouge();
    void devientVert();

public slots:

    //Slots de gestion du client
    void boutonConnexion(); //Connexion au serveur
    void boutonEnvoyer(); //Envoie du message
    void onMessageR(); //Envoie du message
    void donneesRecuesClient(); //Données que recoit un client
    void connecte(); //Client connecté au serveur
    void deconnecte(); //Client seDeconnecte
    void erreurSocket(QAbstractSocket::SocketError erreur); //On affiche les erreurs

    //Slots de gestion du serveur
    void newServeur(); //Appeler lorsqu'on éxécute l'action lancerNouvelleConnexion (comme un constructeur de la fenêtre serveur
    void closeServeur(); //On ferme le serveur
    void nouvelleConnexion(); //Lorsqu'un nouveau client est connecté
    void donneesRecues();
    void deconnexionClient(); //Lorsqu'on éxécute l'action seDeconnecté
    void seDeconnect();


private:

    QTabWidget *onglets;

    QWidget *ongletClient;
    int indClient;
        QLineEdit *pseudo;
        QLineEdit *message;
        QTextEdit *listeMessages;
        QPushButton *envoyer;
        Client *client; //Qui vas envoyer et recevoir des données, répresente le serveur

    QWidget *ongletServeur;
        QLabel *etatServeur;
        QLabel *port;
        QTcpServer *serveur;
        QList<QTcpSocket*> listeClients; //Pour le serveur, contient quint16 taille message et la QTcpSocket
        QListView *vue;
        QStringList listeEtatClient;
        QStringListModel *modele;
        Client *clientServeur;

    QWidget *ongletParametre;
        QLineEdit *IPserveur;
        QSpinBox *PortServeur;
        QPushButton *connexion;


    //Barre Menu
    QMenu *outils;
    QMenu *info;
        QAction *information;

    //Barre d'outils
    //...Actions
    QAction *lancerNouveauServeur;
    QAction *fermerServeur;
    QAction *seConnecter;
    QAction *seDeconnecter;

    //Indexes tab serveur
    int indexServ;

};


#endif // FENETRE_H
