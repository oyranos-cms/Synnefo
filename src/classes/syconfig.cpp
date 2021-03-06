#include "ui_synnefo-config.h"
#include "syconfig.h"

SyConfig::SyConfig( QList <SyModule*> modules, QWidget * parent)
     : QDialog(parent, 0)
{
    ui = new Ui::syConfigDialog();
    ui->setupUi(this);

    module_list = QList <SyModule*> (modules);

    loadState();

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(closeAndSaveDialog()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(closeDialog()));
    connect(ui->availableModuleList, SIGNAL (currentRowChanged (int)),
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
       ( ui->availableModuleList->item(moduleIndex) )->setText(moduleName + " <HIDDEN>");
    else if (newHidingStatus == false)
       ( ui->availableModuleList->item(moduleIndex) )->setText(moduleName);
    
    mainConfig.beginGroup("modulesVisible");
    mainConfig.setValue(moduleName, newHidingStatus); 
    mainConfig.endGroup();
}   


void SyConfig::changeModuleStatus( bool newState )
{
  
    int currentIndex = ui->availableModuleList->currentRow();
        
    if (newState == true) 
       setModuleHiding( currentIndex, true );
    else if (newState == false)  
       setModuleHiding( currentIndex, false );
}


void SyConfig::loadState()
{   
    int i = 0,
        synnefo_module_count = module_list.count();
    QString moduleName = "";
    SyModuleConfig * currentConfig = 0;

    for (i = 0; i < synnefo_module_count; i++)    
    {
      currentConfig = (SyModuleConfig*)module_list.at(i)->getConfigWidget();
      currentConfig->loadWidgetState();
      
      moduleName = currentConfig->getModuleName();
      
      if(currentConfig->isHiding())
        ui->availableModuleList->addItem(moduleName + " <HIDDEN>");
      else
        ui->availableModuleList->addItem(moduleName);  
      

    }
}


void SyConfig::saveState()
{
  
    int i = 0,
        synnefo_module_count = module_list.count();
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
    QWidget * previousWidget = ui->moduleConfigStack->currentWidget();    
    ui->moduleConfigStack->removeWidget( previousWidget );
    
    QString moduleName = (ui->availableModuleList->item(rowIndex))->text();       
    
    int i,
        synnefo_module_count = module_list.count();
    for (i = 0; i < synnefo_module_count; i++)
    {
        if (moduleName == (module_list.at(i))->getName())
        {
            (module_list.at(i))->attachConfigWidget( ui->moduleConfigStack );
            break;
	}
    }
      
    (module_list.at(rowIndex))->attachConfigWidget( ui->moduleConfigStack ); 
    
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
  delete ui;
}
