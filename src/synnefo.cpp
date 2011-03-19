#include "synnefo.h"

Synnefo::Synnefo(QWidget * parent)
    : QMainWindow(parent)
{       
    setupUi(this);  
    
    Frame->hide();
    
    loadSyModules();
    
    connect (syModuleListView, SIGNAL (itemActivated (QListWidgetItem *)),
               this, SLOT (changeModuleSelection (QListWidgetItem *)));
}


// Initialize Synnefo modules.

void Synnefo::loadSyModules()
{
    devicesModule = new SyDevices(0);
    infoModule = new SyInfo(0);
    settingsModule = new SySettings(0);
    
    // Insert additional modules here...
    syModuleListView->addItem(devicesModule->getName());
    syModuleListView->addItem(infoModule->getName());
    syModuleListView->addItem(settingsModule->getName());
}    


// Changes module widget whenever user clicks on a selection.

void Synnefo::changeModuleSelection (QListWidgetItem * moduleSelection)
{   
    // Pop the previous widget off the stackedWidget.
    QWidget * previousWidget = syModuleWidget->currentWidget();    
    syModuleWidget->removeWidget(previousWidget);
        
    // Update the module widget based on user selection.
    if (moduleSelection->text() == "Devices") 
    {
      syModuleWidget->addWidget(devicesModule);      
      Frame->setVisible(devicesModule->isEditable());
    }
    
    else if (moduleSelection->text() == "Information") 
    {
      syModuleWidget->addWidget(infoModule);
      Frame->setVisible(infoModule->isEditable());
    }
    else if (moduleSelection->text() == "Settings") 
    {
      syModuleWidget->addWidget(settingsModule);     
      Frame->setVisible(settingsModule->isEditable());
    }
}


Synnefo::~Synnefo()
{
  
}