 
#ifndef SYMODULE_H
#define SYMODULE_H

#include <QString>
#include <QIcon>
#include <QWidget>
#include <QSettings>

#include <QStackedWidget>


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
    
  
/*** Protected Data Members ****************/

    QString moduleName;
    QString moduleDescription;
    
    QIcon icon;
    
    QSettings * config;     // Module UI settings
    
    bool editable;
    bool modified;

    
    
};

#endif