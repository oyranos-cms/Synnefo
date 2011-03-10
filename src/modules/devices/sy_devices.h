 
#ifndef SY_DEVICES_H
#define SY_DEVICES_H

#include <QApplication>
#include <QSignalMapper>
#include <QWidget>

// Qt Designer code translation.
#include "ui_sy_devices.h"          

class SyDevices : public QWidget
{
    Q_OBJECT
    
  public:
    SyDevices (QWidget * parent);
    ~SyDevices();
    
  private:
    Ui::syDevicesWidget syDevicesUi;            
};

#endif