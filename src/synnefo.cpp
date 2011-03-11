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
    // Load modules.
    QWidget * sdwidget = new QWidget;
    devicesModule = new SyDevices(sdwidget);
    
    QWidget * siwidget = new QWidget;
    infoModule = new SyInfo(siwidget); 
    
    QWidget * sswidget = new QWidget;
    settingsModule = new SySettings(sswidget); 
    
    // Insert additional modules here...
    syMainUi.syModuleListView->addItem(tr("Devices"));
    syMainUi.syModuleListView->addItem(tr("Information"));
    syMainUi.syModuleListView->addItem(tr("Settings"));
}    


// Changes module widget whenever user clicks on a selection.

void Synnefo::changeModuleSelection (QListWidgetItem * moduleSelection)
{   
    // Pop the previous widget off the stackedWidget.
    QWidget * previousWidget = syMainUi.syModuleWidget->currentWidget();    
    syMainUi.syModuleWidget->removeWidget(previousWidget);
        
    // Update the module widget based on user selection.
    if (moduleSelection->text() == "Devices") {
      devicesModule->show();
      syMainUi.syModuleWidget->addWidget(devicesModule);
      
    } else if (moduleSelection->text() == "Information") {            
      infoModule->show();
      syMainUi.syModuleWidget->addWidget(infoModule);
      
    } else if (moduleSelection->text() == "Settings") {            
      settingsModule->show();
      syMainUi.syModuleWidget->addWidget(settingsModule);      
    }    
}


Synnefo::~Synnefo()
{
  
}