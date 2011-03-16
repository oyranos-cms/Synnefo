 
#ifndef SY_SETTINGS_H
#define SY_SETTINGS_H

#include <QApplication>
#include <QSignalMapper>
#include <QWidget>

#include "symodule.h"

// Qt Designer code translation.
#include "ui_sy_settings.h"          

class SySettings : public QWidget, public SyModule
{
    Q_OBJECT
    
  public:
    SySettings (QWidget * parent);
    ~SySettings();
    
  private:
    Ui::sySettingsWidget sySettingsUi;            
};

#endif