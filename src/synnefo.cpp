#include "synnefo.h"

const int module_count = 3;

Synnefo::Synnefo(QWidget * parent)
    : QMainWindow(parent)
{       
    QCoreApplication::setOrganizationName("OpenICC");
    QCoreApplication::setApplicationName("Synnefo");
      
    setupUi(this);  
    
    loadSyModules();
    
    //Frame->setVisible(false);   // Save/Apply buttons set to FALSE.
    
    connect ( syModuleListView, SIGNAL ( currentRowChanged ( int )),
               this, SLOT ( changeModuleSelection ( int )));
    connect( exitButton, SIGNAL( clicked() ), this, SLOT( closeSynnefo() ));
    connect( appSettingsButton, SIGNAL( clicked() ), this, SLOT( openApplicationSettings() ));
}



// Initialize Synnefo modules.
void Synnefo::loadSyModules()
{
    // TODO Find solution to automatically check for # of modules.    
    syModules = new SyModule*[module_count];
    
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
    for (i = 0; i < module_count; i++)
       syModuleListView->addItem(syModules[i]->getName());
    
        config = new SyConfig(syModules, this);
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
      
      moduleDescriptionLabel->setText( syModules[moduleIndex]->getDescription() );
      moduleNameLabel->setText( QString("Synnefo ") + syModules[moduleIndex]->getName() );
    
    /* NOTE: Save/Apply buttons reserved until functionality exists.
      
      
      Frame->setVisible( syModules[ moduleIndex ]->isEditable() );
    */
    }
    // TODO Failure case.
}


void Synnefo::openApplicationSettings()
{
    config->setVisible(true);
}


void Synnefo::closeSynnefo()
{
    close();
}


Synnefo::~Synnefo()
{
    int i;
    for (i = 0; i < module_n; i++)
      delete syModules[i];
}