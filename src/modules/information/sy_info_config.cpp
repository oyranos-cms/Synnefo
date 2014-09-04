
#include "sy_info_config.h"

SyInfoConfig::SyInfoConfig (QWidget * parent, QString moduleId) :
   SyModuleConfig(parent, moduleId)
{
    //attachConfigModule(this);
    viewDetailOptLabel = new QLabel("Show detailed profile information");
    viewDetailOptCheckBox = new QCheckBox();
    //syconfigmoduleHub->setParent(0);
    
    populateOptionGrid();
}

void SyInfoConfig::populateOptionGrid()
{
    syModuleConfigGrid()->setColumnStretch(1, 50);
    
    syModuleConfigGrid()->addWidget(viewDetailOptLabel, 0,0,0);
    syModuleConfigGrid()->addWidget(viewDetailOptCheckBox, 0,1,0);  
}

void SyInfoConfig::saveWidgetState()
{
    this->saveModuleHidingState();
}

void SyInfoConfig::loadWidgetState()
{
    this->loadModuleHidingState();
}

SyInfoConfig::~SyInfoConfig()
{
  
}
