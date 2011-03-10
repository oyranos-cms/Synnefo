 
#ifndef SY_DEVICES_H
#define SY_DEVICES_H

#include <QApplication>
#include <QSignalMapper>
#include <QWidget>

// Qt Designer code translation.
#include "ui_sy_info.h"          

class SyInfo : public QWidget
{
    Q_OBJECT
    
  public:
    SyInfo (QWidget * parent);
    ~SyInfo();
    
  private:
    Ui::syInfoWidget syInfoUi;            
};

#endif