#include <QApplication>
#include "fenetre.h"

int main(int argc, char* argv[])
{
    QApplication app(argc,argv);

    FenetreChat fen;
    fen.show();

    return app.exec ();
}
