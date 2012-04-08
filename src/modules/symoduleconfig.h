 
#ifndef SYMODULECONFIG_H
#define SYMODULECONFIG_H

#include <QSettings>
#include <QWidget>

#include "ui_symoduleconfig.h"

/**

  SyModuleConfig class:
  
  Use as a base class for module configuration classes.  
  
  Each Synnefo module will have its own configuration widget, which must attach
  itself to this base class using 'attachConfigModule()'.
  
*/

class SyModuleConfig : public QWidget, protected Ui::syModuleConfigWidget
{
  Q_OBJECT
  
  public:
    SyModuleConfig(QWidget * parent = 0, QString moduleName = 0);    
    ~SyModuleConfig();
    
    QString getRegistrationId(void) const {return configRegistration;}   
    QString getModuleName(void) const {return moduleNameId;}
    bool isHiding(void) const {return isHidden;}    
    
    virtual void saveWidgetState();    
    virtual void loadWidgetState();    
    
  protected:    
    void attachConfigModule(QWidget* newModule)
                      {syModuleConfigGrid->addWidget(newModule, 0,0,0);}
    
    void setModuleHiding(bool);
    void loadModuleHidingState();
    void saveModuleHidingState();
    
    QSettings moduleConfig;
    QString configRegistration;
    
  private:
    QString moduleNameId;    
    bool isHidden;
};

#endif