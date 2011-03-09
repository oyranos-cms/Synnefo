#include "synnefo.h"



int main (int argc, char * argv[])
{
    QApplication app (argc, argv);
    QWidget * test_ui = new QWidget;
    
    //QPixmap pixmap(":/resources/images/synnefo_splash.jpg");
    //QSplashScreen splash(pixmap);
    //splash.show();
    
    //app.processEvents();

    Synnefo x(test_ui);
    x.show();
    
    //splash.finish(&x);


    return app.exec();
}