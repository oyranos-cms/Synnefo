#include "syconfig.h"

extern int synnefo_module_count;

SyConfig::SyConfig( QList <SyModule*> modules, QWidget * parent) 
     : QDialog(parent, 0)
{
  
    setupUi(this);     
    
    module_list = QList <SyModule*> (modules);
       
    loadState();
    
    connect(okButton, SIGNAL(clicked()), this, SLOT(closeAndSaveDialog()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(closeDialog()));
    connect(availableModuleList, SIGNAL (currentRowChanged (int)),
              this, SLOT (changeModuleConfig (int)));
/*
    connect( hideModuleCheckBox, SIGNAL (clicked (bool)),
              this, SLOT (changeModuleStatus( bool )));*/
}

// Helper function to 
void SyConfig::setModuleHiding (int moduleIndex, bool newHidingStatus)
{
  
    QString moduleName = (module_list.at(moduleIndex))->getName();
    
    (module_list.at(moduleIndex))->setHiding(newHidingStatus);

    if (newHidingStatus == true)
       ( availableModuleList->item(moduleIndex) )->setText(moduleName + " <HIDDEN>");
    else if (newHidingStatus == false)
       ( availableModuleList->item(moduleIndex) )->setText(moduleName);
    
    mainConfig.beginGroup("modulesVisible");
    mainConfig.setValue(moduleName, newHidingStatus); 
    mainConfig.endGroup();
}   


void SyConfig::changeModuleStatus( bool newState )
{
  
    int currentIndex = availableModuleList->currentRow();
        
    if (newState == true) 
       setModuleHiding( currentIndex, true );
    else if (newState == false)  
       setModuleHiding( currentIndex, false );
}


void SyConfig::loadState()
{   
    int i = 0;
    QString moduleName = "";
    SyModuleConfig * currentConfig = 0;
    
    for (i = 0; i < synnefo_module_count; i++)    
    {
      currentConfig = (SyModuleConfig*)module_list.at(i)->getConfigWidget();
      currentConfig->loadWidgetState();
      
      moduleName = currentConfig->getModuleName();
      
      if(currentConfig->isHiding())
        availableModuleList->addItem(moduleName + " <HIDDEN>");
      else
        availableModuleList->addItem(moduleName);  
      

    }
}


void SyConfig::saveState()
{
  
    int i = 0;
    SyModuleConfig * currentConfig = 0;
    
    for (i = 0; i < synnefo_module_count; i++)
    {        
      currentConfig = (SyModuleConfig*)module_list.at(i)->getConfigWidget();
      currentConfig->saveWidgetState();
    }
    
}


void SyConfig::changeModuleConfig( int rowIndex )
{
    // Pop the previous widget off the stackedWidget.
    QWidget * previousWidget = moduleConfigStack->currentWidget();    
    moduleConfigStack->removeWidget( previousWidget );
    
    QString moduleName = (availableModuleList->item(rowIndex))->text();       
    
    int i;
    for (i = 0; i < synnefo_module_count; i++)
    {
        if (moduleName == (module_list.at(i))->getName())
        {
            (module_list.at(i))->attachConfigWidget( moduleConfigStack );
            break;
	}
    }
      
    (module_list.at(rowIndex))->attachConfigWidget( moduleConfigStack ); 
    
    /*
    bool moduleHideState = (module_list.at(rowIndex))->isHiding();
    if (moduleHideState == true) 
      hideModuleCheckBox->setCheckState(Qt::Checked);
    else if (moduleHideState == false) 
      hideModuleCheckBox->setCheckState(Qt::Unchecked); */
}       


void SyConfig::closeDialog()
{    
    close();
}

void SyConfig::closeAndSaveDialog()
{
    saveState();
    close();
}


SyConfig::~SyConfig() 
{        
 

}
