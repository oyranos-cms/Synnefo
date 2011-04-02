#include "synnefo.h"

int main (int argc, char * argv[])
{
    QApplication app (argc, argv);
    QCoreApplication::setOrganizationName("OpenICC");
    QCoreApplication::setApplicationName("Synnefo");
    
    QWidget * widget = new QWidget;
        
    Synnefo synnefoApp(widget);
    synnefoApp.show();
    
    return app.exec();
}