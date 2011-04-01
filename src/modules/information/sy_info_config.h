
#ifndef SY_INFO_CONFIG_H
#define SY_INFO_CONFIG_H

#include "symoduleconfig.h"

#include "ui_sy_info_config.h"
#include "ui_sy_info.h"      

class SyInfoConfig : public SyModuleConfig, Ui::syInfoConfig
{
  Q_OBJECT
  
  public:
    SyInfoConfig(QWidget * parent = 0);
    ~SyInfoConfig();
    
  private:
    Ui::syInfoWidget ui_;    // Variable to access 'Devices' widget objects.
};

#endif