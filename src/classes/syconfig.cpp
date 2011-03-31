#include "syconfig.h"


SyConfig::SyConfig(QString org, QString application) 
          : QSettings (org, application)
{

}

SyConfig::~SyConfig() 
{
  
}