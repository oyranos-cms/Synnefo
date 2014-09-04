 
#ifndef SYCONFIG_H
#define SYCONFIG_H

#include <QWidget>
#include <QSettings>
#include <QDialog>

#include "symodule.h"

namespace Ui {
  class syConfigDialog;
}

class SyConfig : public QDialog
{
  Q_OBJECT
  
  public:
    SyConfig( QList <SyModule*> modules, QWidget * parent);    
    ~SyConfig();
    
    void saveState();
    void loadState();
    
  private slots:
    
    void closeDialog();   
    void closeAndSaveDialog();
    
    void changeModuleConfig( int );
    
    // QCheckBox
    void changeModuleStatus( bool );
    
  private:
    Ui::syConfigDialog * ui;
    
    void setModuleHiding(int, bool);
    
    QSettings mainConfig;    
    
    QList <SyModule*> module_list;
};

#endif
