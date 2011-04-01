 
#ifndef SYMODULECONFIG_H
#define SYMODULECONFIG_H

#include <QSettings>
#include <QWidget>

/**

  SyModuleConfig class:
  
  Use as a base class for module configuration classes.
  
*/

class SyModuleConfig : public QWidget
{
  public:
    SyModuleConfig(QWidget * parent = 0);    
    ~SyModuleConfig();
    
  protected:
    
    void saveWidgetState();
    
    void loadWidgetState();
};

#endif