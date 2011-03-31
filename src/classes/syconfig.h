 
#ifndef SYCONFIG_H
#define SYCONFIG_H

#include <QWidget>
#include <QSettings>

#include "ui_synnefo-config.h"
#include "symodule.h"

class SyConfig : public QDialog, Ui::syConfigDialog
{
  Q_OBJECT
  
  public:
    SyConfig(SyModule ** modules, QWidget * parent);    
    ~SyConfig();
    
  private slots:
    void closeDialog();
   
  private:
    QSettings config_;
};

#endif