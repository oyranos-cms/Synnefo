/*
Copyright (C) 2011-2012 Joseph Simon <jsimon383@gmail.com>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions o9hband the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS ORbsd
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORTcd
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef SYNNEFO_H
#define SYNNEFO_H


#include <QApplication>
#include <QWidget>

// Qt Designer code translation.
#include "ui_synnefo.h"   

// Synnefo Module Headers
#include "sy_database.h"
#include "sy_devices.h"
#include "sy_info.h"
#include "sy_settings.h"

#include "syconfig.h"


/** SynnefoApp class ***************************************

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

class SynnefoApp : public QMainWindow, private Ui::syMainWindow
{
    Q_OBJECT
    
public:
    
    SynnefoApp (QWidget* parent);
    ~SynnefoApp();

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
    
    
    QList <SyModule*> moduleList;       // List to store Synnefo modules.
    
    // Synnefo module widget interfaces.
    SyDatabase* databaseModule;
    SyDevices* devicesModule;          
    SyInfo* infoModule;                
    SySettings* settingsModule;    
      
    SyConfig* configDialog;            // Synnefo config dialog.
    
    bool isFirstRun;                   // Flag to determine first run.
    
    QSettings configuration;           // Variable to save Ui state.
};

#endif