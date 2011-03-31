#ifndef SYNNEFO_H
#define SYNNEFO_H

#include <QApplication>
#include <QSignalMapper>
#include <QWidget>

// Qt Designer code translation.
#include "ui_synnefo.h"   

// Synnefo Module Headers
#include "sy_devices.h"
#include "sy_info.h"
#include "sy_settings.h"

class Synnefo : public QMainWindow, Ui::syMainWindow
{
    Q_OBJECT
    
  public:
    
    Synnefo (QWidget * parent);
    ~Synnefo();
    
    SyModule ** syModules;

  private slots:
    
    // Updates module widget.
    void changeModuleSelection ( int );
    
    void closeSynnefo();
    
  private:
    
    int insertModule( SyModule *, QStackedWidget * );
    
    void loadSyModules();
    
    // Synnefo module interfaces.
    SyDevices * devicesModule;    
    SyInfo * infoModule;
    SySettings * settingsModule;
    
    QList <SyModule*> syModulars;
    
    int module_n;
    
};

#endif