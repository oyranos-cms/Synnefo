#include "synnefo.h"

int main (int argc, char * argv[])
{
    QApplication app (argc, argv);
    QWidget * widget = new QWidget;
    
    Synnefo synnefoApp(widget);
    synnefoApp.show();
    
    return app.exec();
}