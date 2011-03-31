
#include "sy_settings_config.h"

SySettingsConfig::SySettingsConfig (QWidget * parent) :
   SyModuleConfig(parent)
{
    setupUi(this);
    ui_.label_ACTION_OPEN_MISMATCH_RGB->setText("dfa");
}

SySettingsConfig::~SySettingsConfig()
{
  
}