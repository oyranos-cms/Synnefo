#include "symodule.h"

SyModule::SyModule(QWidget * parent)
    : QWidget(parent)
{
    config = new QSettings("Synnefo", "OpenICC");
  
    moduleName = "module";
    moduleDescription = "A Synnefo Module";  
    
    
    modified = false;
}

int SyModule::attachModule( QStackedWidget * sw )
{
    sw->addWidget( this );
    
    return 1;
}

void SyModule::save()
{
    
}

void SyModule::load()
{
  
}