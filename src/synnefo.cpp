#include "synnefo.h"

const int moduleCount = 3;

Synnefo::Synnefo(QWidget * parent)
    : QMainWindow(parent)
{   
    setupUi(this);  
    
    // Check if running Synnefo for first time.
    isFirstRun = configuration.value("first-run", true).toBool();
        
    loadSyModules();
        
    configDialog = new SyConfig(moduleList, this);
    
    connect (syModuleListView, 
	      SIGNAL ( currentRowChanged ( int )), this, 
	      SLOT ( changeModuleSelection ( int )));
    connect( exitButton, SIGNAL( clicked() ), this, SLOT( closeSynnefo() ));
    connect( appSettingsButton, SIGNAL( clicked() ), this, SLOT( openApplicationSettings() ));
}


// Initialize Synnefo modules.
void Synnefo::loadSyModules()
{
    // TODO Find solution to automatically check for # of modules in directory structure.                   
    // Initialize pre-packaged Synnefo modules.
    devicesModule = new SyDevices(0);
    infoModule = new SyInfo(0);
    settingsModule = new SySettings(0);
    
    moduleList.insert( 0, settingsModule );
    moduleList.insert( 0, infoModule );
    moduleList.insert( 0, devicesModule );    
      
    refreshModuleList();    
    
}  


// Changes module widget whenever user clicks on a selection.
void Synnefo::changeModuleSelection ( int rowIndex )
{   
    // Pop the previous widget off the stackedWidget.
    QWidget * previousWidget = syModuleWidget->currentWidget();    
    syModuleWidget->removeWidget( previousWidget );    
  
  
    QString moduleName = (syModuleListView->item(rowIndex))->text();       
    
    int i;
    for (i = 0; i < moduleCount; i++)
    {
        if (moduleName == (moduleList.at(i))->getName())
        {
            (moduleList.at(i))->attachModule( syModuleWidget );
    
            moduleDescriptionLabel->setText( (moduleList.at(i) )->getDescription() );
            moduleNameLabel->setText( QString("Synnefo ") + (moduleList.at(i) )->getName() );

            break;
        }
    }  
}


void Synnefo::openApplicationSettings()
{  
    configDialog->exec();
    
    // FIXME Refreshing list after hiding results in a segfault.
    // refreshModuleList();
}


void Synnefo::refreshModuleList()
{ 
    int i;
    
    syModuleListView->clear();
        
    configuration.beginGroup("modulesVisible");    
    
    for (i = 0; i < moduleCount; i++)
    {        
        QString moduleName = (moduleList.at(i))->getName() ;
        bool hideStatus = configuration.value(moduleName).toBool();
        
        (moduleList.at(i))->setHiding(hideStatus);
    }  
    
    configuration.endGroup();
    
    for (i = 0; i < moduleCount; i++)
    {
       QString moduleName = (moduleList.at(i))->getName() ;
       bool hideStatus = (moduleList.at(i))->isHiding();
       
       if (hideStatus == false)
       {
         QListWidgetItem * item = new QListWidgetItem(moduleName);
         syModuleListView->addItem(item);        
       }
    }
         
    
}


void Synnefo::saveState()
{


}


void Synnefo::closeSynnefo()
{
    saveState();
    
    if (isFirstRun) 
       configuration.setValue("first-run", false);
  
    close();
}


Synnefo::~Synnefo()
{
   while (!moduleList.isEmpty())
     delete moduleList.takeFirst();
}
