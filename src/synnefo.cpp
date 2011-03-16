#include "synnefo.h"

Synnefo::Synnefo(QWidget * parent)
    : QMainWindow(parent)
{       
    syMainUi.setupUi(this);  
    
    loadSyModules();
    
    connect (syMainUi.syModuleListView, SIGNAL (itemActivated (QListWidgetItem *)),
               this, SLOT (changeModuleSelection (QListWidgetItem *)));
}


// Initialize Synnefo modules.

void Synnefo::loadSyModules()
{
    devicesModule = new SyDevices(0);
    infoModule = new SyInfo(0);
    settingsModule = new SySettings(0);
    
    // Insert additional modules here...
    syMainUi.syModuleListView->addItem(devicesModule->getName());
    syMainUi.syModuleListView->addItem(infoModule->getName());
    syMainUi.syModuleListView->addItem(settingsModule->getName());
}    


// Changes module widget whenever user clicks on a selection.

void Synnefo::changeModuleSelection (QListWidgetItem * moduleSelection)
{   
    // Pop the previous widget off the stackedWidget.
    QWidget * previousWidget = syMainUi.syModuleWidget->currentWidget();    
    syMainUi.syModuleWidget->removeWidget(previousWidget);
        
    // Update the module widget based on user selection.
    if (moduleSelection->text() == "Devices") 
      syMainUi.syModuleWidget->addWidget(devicesModule);      
    else if (moduleSelection->text() == "Information") 
      syMainUi.syModuleWidget->addWidget(infoModule);
    else if (moduleSelection->text() == "Settings") 
      syMainUi.syModuleWidget->addWidget(settingsModule);      
        
}


Synnefo::~Synnefo()
{
  
}