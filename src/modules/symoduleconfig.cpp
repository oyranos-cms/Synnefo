 
#include "symoduleconfig.h"

SyModuleConfig::SyModuleConfig(QWidget * parent, QString moduleName) : QWidget(parent)
{
    moduleNameId = moduleName;
    configRegistration = QString("synnefo/modules/" + moduleNameId + "/config/");
    
    setupUi(this);
    
    loadModuleHidingState();
}

void SyModuleConfig::saveModuleHidingState()
{
    isHidden = hideModuleCheckBox->isChecked();
  
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
      hideModuleCheckBox->setCheckState(Qt::Checked);
    else if (isHidden == false)
      hideModuleCheckBox->setCheckState(Qt::Unchecked);
}

SyModuleConfig::~SyModuleConfig()
{
    
}