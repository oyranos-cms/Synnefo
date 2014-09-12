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

#include "synnefo.h"
#include <oyranos.h>

extern "C" {
int oyGuiMessageFunc( int code, void * c, const char * format, ... );
}

SynnefoApp::SynnefoApp(QWidget * parent)
    : QMainWindow(parent)
{   
    oyMessageFuncSet( oyGuiMessageFunc );

    setupUi(this);        // Initialize main Synnefo Ui.

    // Check if running is Synnefo for the first time.
    isFirstRun = configuration.value("first-run", true).toBool();
   
    loadSyModules();
    
    // NOTE We shall hide the "Application Settings" until the code is done.
    appSettingsButton->setHidden(true);
        
    //m_synnefo = new Synnefo();
    //confiobject->exec();
    
    connect(syModuleListView, 
            SIGNAL (currentRowChanged (int)), 
            this, 
            SLOT (changeModuleSelection(int)));
    connect(exitButton, 
            SIGNAL( clicked() ), 
            this, 
            SLOT( closeSynnefo() ));
    connect(appSettingsButton, 
            SIGNAL( clicked() ), 
            this, 
            SLOT( openApplicationSettings() ));
}



//   ********************************
//   *    PRIVATE SLOT FUNCTIONS    *
//   ********************************


void SynnefoApp::changeModuleSelection(int rowIndex)
/*  Pre: The user clicks on a module item in the selection list.
    Post: The function passes the index that is clicked by the user (rowIndex),
          changing the appropriate widget item. */
{   
    // Pop the previous widget off the stackedWidget.
    QWidget * previousWidget = syModuleWidget->currentWidget();    
    syModuleWidget->removeWidget( previousWidget );    
    
    QString moduleName = (syModuleListView->item(rowIndex))->text();       
    
    int i,
        synnefo_module_count = moduleList.count();
    for (i = 0; i < synnefo_module_count; i++) {
        if (moduleName == (moduleList.at(i))->getName()) {

            (moduleList.at(i))->attachModule( syModuleWidget );
    
            // Display module information above its widget.
            moduleDescriptionLabel->
                           setText((moduleList.at(i) )->getDescription() );
            moduleNameLabel->setText( (moduleList.at(i) )->getName() );
        }
    }  
}



void SynnefoApp::openApplicationSettings()
/*  Pre: The user clicks on the "Application Settings" button in the main dialog.
    Post: Opens the Synnefo Configuration dialog. */
{  
    // "Application Settings"
    configDialog = new SyConfig(moduleList, this);    
    configDialog->setAttribute(Qt::WA_DeleteOnClose);
    
    configDialog->exec();
    configDialog = 0;
    
    // FIXME Refreshing list after changing hiding status in a segfault.
    // refreshModuleList();
}



void SynnefoApp::closeSynnefo()
/*  Pre: The user clicks on the "Exit" button in the main dialog.
    Post: Closes the Synnefo dialog, and saves the Ui state upon exiting. 
          The isFirstRun flag will be set to FALSE if it isn't already. */
{
    saveState();

    if (isFirstRun)
       configuration.setValue("first-run", false);

    close();
}



//   ***************************
//   *    PRIVATE FUNCTIONS    *
//   ***************************


void SynnefoApp::loadSyModules()
/*  Pre: SyModule objects and module list are unintialized.
    Post: Initializes each SyModule objects and populates the module listing.
          The module selection in the main Synnefo dialog will be refreshed.*/
{
    // TODO Find solution to automatically check
    //      for # of modules in directory structure.

    //databaseModule = new SyDatabase(0);     // "Synnefo Database"
    devicesModule = new SyDevicesModule(0);       // "Synnefo Devices"
    settingsModule = new SySettingsModule(0);     // "Synnefo Settings"
    infoModule = new SyInfoModule(0);             // "Synnefo Information"

    moduleList.insert( 0, settingsModule );
    moduleList.insert( 1, devicesModule );
    moduleList.insert( 2, infoModule );
    //moduleList.insert( 0, databaseModule );

    refreshModuleList();
}  



void SynnefoApp::refreshModuleList()
/*  Post: Updates the module listing in the Synnefo dialog. */
{
    int i = 0,
        synnefo_module_count = moduleList.count();
    QString moduleName = "";
    SyModule * currentModule = 0;

    syModuleListView->clear();

    for(i = 0; i < synnefo_module_count; i++){
      currentModule = moduleList.at(i);
      moduleName = currentModule->getName();

      if(!currentModule->isHiding()){
        QListWidgetItem * item = new QListWidgetItem(moduleName);
        syModuleListView->addItem(item);
      }
    }
}



void SynnefoApp::saveState()
/* Post: Saves the Ui state configuration. */
{


}



void SynnefoApp::freeSyModules()
/* Pre: Initialized SyModules and SyModule QList.
   Post: Frees allocated memory from the SyModule list,
         including the individual modules contained in it. */
{
   while (!moduleList.isEmpty())
     delete moduleList.takeFirst();

   //databaseModule = 0;
   devicesModule = 0;
   infoModule = 0;
   settingsModule = 0;
}



SynnefoApp::~SynnefoApp()
{
    freeSyModules();
}
