
#include "sy_devices_config.h"

#include "ui_sy_devices_config.h"

SyDevicesConfig::SyDevicesConfig (QWidget * parent, QString moduleId) :
   SyModuleConfig(parent, moduleId)
{

}

void SyDevicesConfig::loadWidgetState()
{
   this->loadModuleHidingState();
}

void SyDevicesConfig::saveWidgetState()
{
   this->saveModuleHidingState();
}

SyDevicesConfig::~SyDevicesConfig()
{
  
}
