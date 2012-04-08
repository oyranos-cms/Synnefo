
#ifndef SY_INFO_CONFIG_H
#define SY_INFO_CONFIG_H

#include "symoduleconfig.h"

#include "ui_sy_info_config.h"

class SyInfoConfig : public SyModuleConfig
{
  Q_OBJECT
  
  public:
    SyInfoConfig(QWidget * parent = 0, QString moduleId = "");
    ~SyInfoConfig();    
    
    Ui::syInfoConfig ui_;
    
    void saveWidgetState();
    void loadWidgetState();
    
  private:
    void populateOptionGrid();
    
    // Detailed view options.
    QLabel * viewDetailOptLabel;
    QCheckBox * viewDetailOptCheckBox;
};

#endif