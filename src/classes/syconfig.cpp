#include "syconfig.h"

const int module_count = 3;

SyConfig::SyConfig( QList <SyModule*> modules, QWidget * parent) : QDialog(parent, 0)
{
    setupUi(this); 
    
    SyModule * tempItem = 0;
    
    int i = 0;
    for (i = 0; i < module_count; i++) {
      tempItem = modules.at(i);
      module_list.insert(module_count, tempItem);
      
      availableModuleList->addItem( (modules.at(i))->getName() ); 
    }
    
    //moduleConfigStack->addWidget(modules[2]->getConfigWidget() );
    
    connect( closeButton, SIGNAL( clicked() ), this, SLOT( closeDialog() ));
    connect ( availableModuleList, SIGNAL ( currentRowChanged ( int )),
              this, SLOT ( changeModuleConfig ( int )));
}


void SyConfig::changeModuleConfig( int moduleIndex )
{
    // Pop the previous widget off the stackedWidget.
    QWidget * previousWidget = moduleConfigStack->currentWidget();    
    moduleConfigStack->removeWidget( previousWidget );
      
    (module_list.at(moduleIndex))->attachConfigWidget( moduleConfigStack ); 
}


void SyConfig::closeDialog()
{
    close();
}


SyConfig::~SyConfig() 
{
    module_list.clear();
}
