#include "fenetre.h"

FenetreChat::FenetreChat() : QMainWindow ()
{

    initialisationVariable ();
    creationAction ();
    creationMenu ();
    creationBarreOutils ();

    //Les onglets
    dessinClient ();
    dessinServeur ();
    dessinParametre ();

    devientRouge ();
    setWindowIcon (QIcon("Log2.png"));
    setWindowTitle ("Ster-Chat");
    setMinimumSize (640,480);
    setFont (QFont("Times New Roman",14));

    connexionSlots ();
}


void FenetreChat::initialisationVariable()
{
    onglets = new QTabWidget;

        pseudo = new QLineEdit;
        pseudo->setPlaceholderText ("Votre pseudo");
        message = new QLineEdit;
        envoyer = new QPushButton("Envoyer");

        listeMessages = new QTextEdit;
        listeMessages->setReadOnly (true);

        IPserveur = new QLineEdit;
        IPserveur->setPlaceholderText ("Ex: 127.0.0.1");
        PortServeur = new QSpinBox;
        PortServeur->setRange (1024,65535);
        PortServeur->setValue (50000);
        connexion = new QPushButton("Connexion");

        //listeMessages->setEnabled (false); // Lorsqu'on n'est pas connecté

    setCentralWidget (onglets);
}

void FenetreChat::creationAction()
{
    seConnecter = new QAction("Connexion");
    seConnecter->setIcon (QIcon("search.png"));
    seDeconnecter = new QAction("Deconnexion");
    seDeconnecter->setIcon (QIcon("off.png"));
    lancerNouveauServeur = new QAction("Lancer Serveur !");
    lancerNouveauServeur->setIcon (QIcon("new.png"));
    fermerServeur = new QAction("Fermer !");
    fermerServeur->setIcon (QIcon("stop.png"));
    information = new QAction("A propos...");

    //On réalise toutes les connexions nécéssaires
    connect (lancerNouveauServeur,SIGNAL(triggered()),this,SLOT(newServeur()));
    connect (fermerServeur,SIGNAL(triggered()),this,SLOT(closeServeur()));
}

void FenetreChat::creationMenu()
{
    outils = menuBar ()->addMenu ("Outils");
        outils->addAction (seConnecter);
        outils->addAction (seDeconnecter);
        outils->addAction (lancerNouveauServeur);
        outils->addAction (fermerServeur);
    info = menuBar ()->addMenu ("Aide...");
        info->addAction (information);
}

void FenetreChat::creationBarreOutils()
{
    QToolBar *barreOutils = addToolBar ("outils");

    barreOutils->addAction (seConnecter);
    barreOutils->addAction (seDeconnecter);
    barreOutils->setMaximumHeight (26);
}


void FenetreChat::connexionSlots()
{
    connect (seDeconnecter,SIGNAL(triggered()),this,SLOT(seDeconnect()));
    connect (seConnecter,SIGNAL(triggered()),this,SLOT(boutonConnexion()));
}



//...Rouge on n'est pas connecté
void FenetreChat::devientRouge()
{
    QColor couleur(Qt::darkRed);
    QPalette palette;
    palette.setColor (QPalette::Base,couleur);
    listeMessages->setPalette (palette);
    envoyer->setEnabled (false);
}

void FenetreChat::devientVert()
{
    QColor couleur(Qt::green);
    QPalette palette;
    palette.setColor (QPalette::Base,couleur);
    listeMessages->setPalette (palette);
    envoyer->setEnabled (true);
}
