
#include "sy_settings_config.h"

SySettingsConfig::SySettingsConfig (QWidget * parent, QString moduleId) :
   SyModuleConfig(parent, moduleId)
{
    moduleConfig.beginGroup("Settings");   
    moduleConfig.endGroup();
}


void SySettingsConfig::loadWidgetState()
{
   this->loadModuleHidingState();
}

void SySettingsConfig::saveWidgetState()
{
   this->saveModuleHidingState();
}


SySettingsConfig::~SySettingsConfig()
{
  
}