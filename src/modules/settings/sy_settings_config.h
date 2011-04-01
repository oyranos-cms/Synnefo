
#ifndef SY_SETTINGS_CONFIG_H
#define SY_SETTINGS_CONFIG_H

#include "symoduleconfig.h"



#include "ui_sy_settings_config.h"
#include "ui_sy_settings.h"      

class SySettingsConfig : public SyModuleConfig, Ui::sySettingsConfig
{
  Q_OBJECT
  
  public:
    SySettingsConfig(QWidget * parent = 0);
    ~SySettingsConfig();
    
  private:
    Ui::sySettingsWidget ui_;    // Variable to access 'Settings' widget objects.
};

#endif