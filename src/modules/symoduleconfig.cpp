 
#include "symoduleconfig.h"
#include "ui_symoduleconfig.h"

SyModuleConfig::SyModuleConfig(QWidget * parent, QString moduleName) : QWidget(parent)
{
    moduleNameId = moduleName;
    configRegistration = QString("synnefo/modules/" + moduleNameId + "/config/");
    
    ui = new Ui::syModuleConfigWidget();
    ui->setupUi(this);
    
    loadModuleHidingState();
}

void SyModuleConfig::saveModuleHidingState()
{
    isHidden = ui->hideModuleCheckBox->isChecked();
  
    moduleConfig.beginGroup(configRegistration);
    moduleConfig.setValue("is_hidden", isHidden);
    moduleConfig.endGroup();
}


void SyModuleConfig::loadModuleHidingState()
{
    moduleConfig.beginGroup(configRegistration);
    isHidden = moduleConfig.value("is_hidden").toBool();
    moduleConfig.endGroup();
    
    if(isHidden == true)
      ui->hideModuleCheckBox->setCheckState(Qt::Checked);
    else if (isHidden == false)
      ui->hideModuleCheckBox->setCheckState(Qt::Unchecked);
}

void SyModuleConfig::attachConfigModule(QWidget* newModule)
{
  ui->syModuleConfigGrid->addWidget(newModule, 0,0,0);
}

QGridLayout * SyModuleConfig::syModuleConfigGrid()
{
  return ui->syModuleConfigGrid;
}

void SyModuleConfig::saveWidgetState()
{
  
}

void SyModuleConfig::loadWidgetState()
{
  
}

SyModuleConfig::~SyModuleConfig()
{
  delete ui;   
}
