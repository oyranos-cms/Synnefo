 
#include "sy_settings.h"

SySettings::SySettings(QWidget * parent)
    : QWidget(parent), SyModule()
{       
    this->moduleName = "Information";
    sySettingsUi.setupUi(this);
}

SySettings::~SySettings()
{
  
}