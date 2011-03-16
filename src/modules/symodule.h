 
#ifndef SYMODULE_H
#define SYMODULE_H

#include <QString>
#include <QIcon>
#include <QWidget>

class SyModule
{
  public:

    QString moduleName;
    QIcon icon;
        
    SyModule();
    QString getName(){ return moduleName; } 

};

#endif