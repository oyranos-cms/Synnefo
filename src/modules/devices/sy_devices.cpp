 
#include "sy_devices.h"

SyDevices::SyDevices(QWidget * parent)
    : QWidget(parent), SyModule()
{       
    this->moduleName = "Devices";
    
    syDevicesUi.setupUi(this);
}

SyDevices::~SyDevices()
{
  
}