
#ifndef SY_INFO_CONFIG_H
#define SY_INFO_CONFIG_H

#include <QCheckBox>
#include <QLabel>

#include "symoduleconfig.h"

class SyInfoConfig : public SyModuleConfig
{
  Q_OBJECT
  
  public:
    SyInfoConfig(QWidget * parent = 0, QString moduleId = "");
    ~SyInfoConfig();    
    
    void saveWidgetState();
    void loadWidgetState();
    
  private:
    void populateOptionGrid();
    
    // Detailed view options.
    QLabel * viewDetailOptLabel;
    QCheckBox * viewDetailOptCheckBox;
};

#endif
