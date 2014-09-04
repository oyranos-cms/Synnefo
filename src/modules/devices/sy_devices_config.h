
#ifndef SY_DEVICES_CONFIG_H
#define SY_DEVICES_CONFIG_H

#include "symoduleconfig.h"


class SyDevicesConfig : public SyModuleConfig
{
  Q_OBJECT

  public:
    SyDevicesConfig(QWidget * parent = 0, QString moduleId = 0);
    ~SyDevicesConfig();
 
    void loadWidgetState();
    void saveWidgetState();

  private:
    //Ui::syDevicesWidget ui_;    // Variable to access 'Devices' widget objects.
};

#endif
