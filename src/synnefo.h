//  "synnefo.h"

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


/** Synnefo class ***************************************

AUTHOR:
  Joseph Simon
  
DESCRIPTION: 

    This is the main application class for Synnefo.  
  It inherits from QMainWindow and the Qt Designer Ui file
  'synnefo.ui'.  The 'main' source file will instantiate
  a Synnefo object, which requires passing a QWidget object.
  Synnefo will then be displayed using the "show()" method.  
  
    The color management modules that are used in Synnefo
  (SyModule) are instantiated here, as well as the 
  application-specific configuration dialog (SyConfig).
  
  
**********************************************************/

class Synnefo : public QMainWindow, private Ui::syMainWindow
{
    Q_OBJECT
    
public:
    
    Synnefo (QWidget* parent);
    ~Synnefo();
    

private slots:    
    
    void changeModuleSelection ( int );
    
    void closeSynnefo();
    
    void freeSyModules();
    
    void openApplicationSettings();
    
    void refreshModuleList();
    
    
private:      
  
//  *** Private Functions *********************    
    
    void loadSyModules();
    
    void saveState();
    
    
//  *** Private Data Members ******************    
    
    // List to store Synnefo modules.
    QList <SyModule*> moduleList;
    
    // Synnefo module widget interfaces.
    SyDevices* devicesModule;    
    SyInfo* infoModule;
    SySettings* settingsModule;
      
    // Synnefo config dialog.
    SyConfig* configDialog;
    
    // Flag to determine first run of the application.
    bool isFirstRun;  
        
    // Variable to save Ui state.
    QSettings configuration;
};

#endif