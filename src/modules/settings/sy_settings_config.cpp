
#include "sy_settings_config.h"

SySettingsConfig::SySettingsConfig (QWidget * parent) :
   SyModuleConfig(parent)
{
    setupUi(this);
        
    moduleConfig.beginGroup("Settings");   
    moduleConfig.endGroup();
}


void SySettingsConfig::readSettings()
{
     
}


SySettingsConfig::~SySettingsConfig()
{
  
}