#include "symodule.h"


SyModule::SyModule(QWidget * parent)
    : QWidget(parent)
{
    index_ = -1;
  
    setModuleName("module");
    setDescription("A Synnefo Module");      
    
    config_ = NULL;
    modified = false;
    
    hiding = false;
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

void SyModule::setHiding( bool b )
{
   //config_->setModuleHiding(b);   
   hiding = b;
}


void SyModule::save()
{
    
}


void SyModule::load()
{
  
}


SyModule::~SyModule()
{
   delete config_;
}