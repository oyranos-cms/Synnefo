#include "syconfig.h"

const int module_count = 3;

SyConfig::SyConfig( SyModule ** modules, QWidget * parent) : QDialog(parent, 0)
{
    setupUi(this); 
    
    int i = 0;
    for (i = 0; i < module_count; i++)    
      availableModuleList->addItem( modules[i]->getName() );    
    
    connect( closeButton, SIGNAL( clicked() ), this, SLOT( closeDialog() ));
}


void SyConfig::closeDialog()
{
    close();
}


SyConfig::~SyConfig() 
{
  
}
