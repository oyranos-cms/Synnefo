 
#include "sy_info.h"

SyInfo::SyInfo(QWidget * parent)
    : QWidget(parent), SyModule()
{       
    this->moduleName = "Settings";
    syInfoUi.setupUi(this);
}

SyInfo::~SyInfo()
{
  
}