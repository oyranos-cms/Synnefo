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

    devicesModule = new SyDevices( NULL );
    infoModule = new SyInfo( NULL );
    settingsModule = new SySettings( NULL );
    
    moduleList.insert(module_count, devicesModule);
    moduleList.insert(module_count, infoModule);
    moduleList.insert(module_count, settingsModule);
        
    int i;
    // Populate Module Selection List 
    for (i = 0; i < module_count; i++)
    { 
       syModuleListView->addItem( (moduleList.at(i))->getName() );       
    }
}    


// Changes module widget whenever user clicks on a selection.
void Synnefo::changeModuleSelection ( int moduleIndex )
{   
    if (moduleIndex != -1) 
    {
      // Pop the previous widget off the stackedWidget.
      QWidget * previousWidget = syModuleWidget->currentWidget();    
      syModuleWidget->removeWidget( previousWidget );
      
      (moduleList.at(moduleIndex))->attachModule( syModuleWidget );   
      
      moduleDescriptionLabel->setText( (moduleList.at(moduleIndex) )->getDescription() );
      moduleNameLabel->setText( QString("Synnefo ") + (moduleList.at(moduleIndex) )->getName() );
    
    }
    // TODO Failure case.
}


void Synnefo::openApplicationSettings()
{
    config = new SyConfig(moduleList, this);
    config->setVisible(true);
}


void Synnefo::closeSynnefo()
{
    close();
}


Synnefo::~Synnefo()
{
    moduleList.clear();
}