 
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
    SyConfig( QList <SyModule*> modules, QWidget * parent);    
    ~SyConfig();
    
  private slots:
    void closeDialog();   
    
    void changeModuleConfig( int );
    
  private:
    QList <SyModule*> module_list;
};

#endif