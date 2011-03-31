 
#ifndef SYCONFIG_H
#define SYCONFIG_H

#include <QSettings>
#include <QString>

class SyConfig : public QSettings
{
  public:
    
    SyConfig(QString org, QString application);
    ~SyConfig();
   
  
};

#endif