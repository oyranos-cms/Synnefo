 
#ifndef SYMODULE_H
#define SYMODULE_H

#include <QString>
#include <QIcon>
#include <QWidget>



/** Synnefo Module class **************************************************

DESCRIPTION: 

  SyModule is the general class for Synnefo modules.  
              
  Each individual module is assigned different color-management roles, 
  but each will share everything that is defined here (so long as they inherit this class).  
  
******************************************************************************/


class SyModule 
{
  
public:
     
    SyModule();
    
    
/*** Public Functions ****************/    
    
    // Get module name.
    QString getName(){ return moduleName; } 
    
    // Returns current 'editable' state; 
    bool isEditable() {return editable;}    
    
    // Sets 'editable' state.
    void setEditable (bool b) { editable = b; }    
    
    // Returns current 'modified' state.
    bool isModified(void) { return modified; }
    
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
    
    bool editable;
    bool modified;

    
    
};

#endif