#include "fenetre.h"

void FenetreChat::dessinParametre()
{
    ongletParametre = new QWidget;

    QFormLayout *form = new QFormLayout;

    form->addRow ("Adresse IP de l'hôte : ",IPserveur);
    form->addRow ("Port de communication : ",PortServeur);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout (form);
    layout->addWidget (connexion);


    ongletParametre->setLayout (layout);
    onglets->addTab (ongletParametre,"Paramètres");

    //Paramètres
    connect (connexion,SIGNAL(clicked()),this,SLOT(boutonConnexion()));
}


void FenetreChat::boutonConnexion ()
{
    onglets->setCurrentIndex (indClient);

    listeMessages->append ("<em>Tentative de connexion...</em>");

    connexion->setEnabled (false);

    //On supprime les possibles connexions précédente
    client->getSocketClient ()->abort ();

    //On se connecte
    client->getSocketClient ()->connectToHost (IPserveur->text (),PortServeur->value ());

}

