 
#ifndef SYMODULE_H
#define SYMODULE_H

#include <QString>
#include <QIcon>
#include <QWidget>
#include <QSettings>

#include <QStackedWidget>

#include "symoduleconfig.h"


/** Synnefo Module class **************************************************

DESCRIPTION: 

  SyModule is the general class for Synnefo modules.  
              
  Each individual module is assigned different color-management roles, 
  but each will share everything that is defined here (so long as they inherit this class).  
  
******************************************************************************/


class SyModule : public QWidget
{
  
public:
     
    SyModule(QWidget * parent);
    
    SyModule ** insertModule( SyModule );


    
/*** Public Functions ****************/    

    // Attach module to an appropriate QStackedWidget.
    int attachModule( QStackedWidget * );
    
    void attachConfigWidget(QStackedWidget * sw);
    
    QWidget * getConfigWidget () { return config_; }
        
    QString getDescription() { return moduleDescription; }
    
    // Get module name.
    QString getName(){ return moduleName; } 
    
    // Returns current 'editable' state; 
    bool isEditable() {return editable;}    
    
    // Returns current 'modified' state.
    bool isModified(void) { return modified; }
    
    // Sets 'editable' state.
    void setEditable (bool b) { editable = b; }        
    
    // Sets 'modified' state.
    void setModified(bool b) { modified = b; }
    
  
protected:
    
/*** Protected Functions ****************/    

    void save();
  
    void load();
        
    void setDescription( QString name ) { moduleDescription = name; }
    
    void setModuleName( QString name ) { moduleName = name; }    
    
    void setConfigWidget( SyModuleConfig * cfg );
    
  
/*** Protected Data Members ****************/

    SyModuleConfig * config_;     // Module config ui.

    QString moduleName;
    QString moduleDescription;
    
    QString moduleHeader;
    
    QIcon icon;
    
    bool editable;
    bool modified;    
    
};

#endif