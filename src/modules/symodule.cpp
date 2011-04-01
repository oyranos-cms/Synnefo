#include "symodule.h"


SyModule::SyModule(QWidget * parent)
    : QWidget(parent)
{
  
    setModuleName("module");
    setDescription("A Synnefo Module");      
    
    config_ = NULL;
    modified = false;
}


int SyModule::attachModule( QStackedWidget * sw )
{
    sw->addWidget( this );
    
    return 1;
}


void SyModule::attachConfigWidget(QStackedWidget * sw)
{
    sw->addWidget( config_ );  
}

void SyModule::setConfigWidget( SyModuleConfig * cfg )
{
    config_ = cfg;
}


void SyModule::save()
{
    
}


void SyModule::load()
{
  
}