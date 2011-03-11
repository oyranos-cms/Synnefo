 
#ifndef SY_INFO_H
#define SY_INFO_H

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