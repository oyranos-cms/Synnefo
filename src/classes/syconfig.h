 
#ifndef SYCONFIG_H
#define SYCONFIG_H

#include <QWidget>
#include <QSettings>
#include <QCheckBox>

#include "ui_synnefo-config.h"
#include "symodule.h"

class SyConfig : public QDialog, Ui::syConfigDialog
{
  Q_OBJECT
  
  public:
    SyConfig( QList <SyModule*> modules, QWidget * parent);    
    ~SyConfig();
    
    void saveState();
    void loadState();
    
  private slots:
    
    void closeDialog();   
    
    void changeModuleConfig( int );
    
    // QCheckBox
    void changeModuleStatus( bool );
    
  private:
    
    void setModuleHiding(int, bool);
    
    QSettings mainConfig;    
    
    QList <SyModule*> module_list;
};

#endif