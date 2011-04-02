#ifndef SYNNEFO_H
#define SYNNEFO_H

#include <QApplication>
#include <QWidget>

// Qt Designer code translation.
#include "ui_synnefo.h"   

// Synnefo Module Headers
#include "sy_devices.h"
#include "sy_info.h"
#include "sy_settings.h"

#include "syconfig.h"

class Synnefo : public QMainWindow, Ui::syMainWindow
{
    Q_OBJECT
    
  public:
    
    Synnefo (QWidget * parent);
    ~Synnefo();

  private slots:
    
    // Updates module widget.
    void changeModuleSelection ( int );
    
    void closeSynnefo();
    
    void openApplicationSettings();
    
    void refreshModuleList();
    
  private:    
    
    void loadSyModules();
    
    void saveState();
    
    // Synnefo module interfaces.
    SyDevices * devicesModule;    
    SyInfo * infoModule;
    SySettings * settingsModule;
    
    SyConfig * configDialog;
    
    bool isFirstRun;
    
    QSettings configuration;
    
    QList <SyModule*> moduleList;
    
};

#endif