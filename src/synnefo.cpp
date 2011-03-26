#include "synnefo.h"

Synnefo::Synnefo(QWidget * parent)
    : QMainWindow(parent)
{       
    setupUi(this);  
    
    loadSyModules();
    
    connect ( syModuleListView, SIGNAL ( currentRowChanged ( int )),
               this, SLOT ( changeModuleSelection ( int )));
}



// Initialize Synnefo modules.
void Synnefo::loadSyModules()
{
    // TODO Find solution to automatically check for # of modules.
    module_n = 3;
    
    syModules = new SyModule*[3];
    
    devicesModule = new SyDevices( NULL );
    infoModule = new SyInfo( NULL );
    settingsModule = new SySettings( NULL );
    
    //syModulars.insert((SyModule*) devicesModule);
    
    // "Module Registration"
    syModules[0] = devicesModule;      /* Devices Module     */ 
    syModules[1] = infoModule;         /* Information Module */ 
    syModules[2] = settingsModule;     /* Settings Module    */ 
        
    int i;
    // Populate Module Selection List 
    for (i = 0; i < module_n; i++)
       syModuleListView->addItem(syModules[i]->getName());
}    


// Changes module widget whenever user clicks on a selection.
void Synnefo::changeModuleSelection ( int moduleIndex )
{   
    if (moduleIndex != -1) 
    {
      // Pop the previous widget off the stackedWidget.
      QWidget * previousWidget = syModuleWidget->currentWidget();    
      syModuleWidget->removeWidget( previousWidget );
    
      syModules[ moduleIndex ]->attachModule( syModuleWidget );    
      Frame->setVisible( syModules[ moduleIndex ]->isEditable() );
    }
    // TODO Failure case.
}


Synnefo::~Synnefo()
{
    int i;
    for (i = 0; i < module_n; i++)
      delete syModules[i];
}