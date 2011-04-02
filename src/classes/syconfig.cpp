#include "syconfig.h"

const int module_count = 3;

SyConfig::SyConfig( QList <SyModule*> modules, QWidget * parent) 
     : QDialog(parent, 0)
{
  
    setupUi(this);     
    
    module_list = QList <SyModule*> (modules);
       
    loadState();
    
    connect( closeButton, SIGNAL( clicked() ), this, SLOT( closeDialog() ));
    connect ( availableModuleList, SIGNAL ( currentRowChanged ( int )),
              this, SLOT ( changeModuleConfig ( int )));
    connect( hideModuleCheckBox, SIGNAL (clicked (bool)),
              this, SLOT (changeModuleStatus( bool )));
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
  
    mainConfig.beginGroup("modulesVisible");
    
    int i;
    for (i = 0; i < module_count; i++)    
    {
      QString moduleName = (module_list.at(i))->getName();
      bool isHidden = mainConfig.value(moduleName).toBool();
      
      if (isHidden == true)
        availableModuleList->addItem( moduleName + " <HIDDEN>");      
      else 
        availableModuleList->addItem( moduleName );     
    }
    
    mainConfig.endGroup();
}


void SyConfig::saveState()
{
  
    int i = 0;
    int visibleCount = 0;
    
    for (i; i < module_count; i++)
    {        
        QString moduleName = (module_list.at(i))->getName() ;
        bool hidingStatus = (module_list.at(i))->isHiding() ;
        
        if (hidingStatus == false)        
            visibleCount++;
    }
    
    mainConfig.setValue("modulesVisibleCount", visibleCount);
    mainConfig.sync();
    
}


void SyConfig::changeModuleConfig( int rowIndex )
{
    // Pop the previous widget off the stackedWidget.
    QWidget * previousWidget = moduleConfigStack->currentWidget();    
    moduleConfigStack->removeWidget( previousWidget );
    
    QString moduleName = (availableModuleList->item(rowIndex))->text();       
    
    int i;
    for (i = 0; i < module_count; i++)
    {
        if (moduleName == (module_list.at(i))->getName())
        {
            (module_list.at(i))->attachConfigWidget( moduleConfigStack );
            break;
	}
    }
      
    (module_list.at(rowIndex))->attachConfigWidget( moduleConfigStack ); 
    
    bool moduleHideState = (module_list.at(rowIndex))->isHiding();
    
    if (moduleHideState == true) 
      hideModuleCheckBox->setCheckState(Qt::Checked);
    else if (moduleHideState == false) 
      hideModuleCheckBox->setCheckState(Qt::Unchecked); 
}       


void SyConfig::closeDialog()
{

    saveState();
    close();
}


SyConfig::~SyConfig() 
{        
 

}
