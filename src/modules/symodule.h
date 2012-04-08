 
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
    ~SyModule();
    
/*** Public Functions ****************/    

    // Attach module to an appropriate QStackedWidget.
    int attachModule( QStackedWidget * );
    
    void attachConfigWidget(QStackedWidget * sw);
    
    QWidget * getConfigWidget () { return config_; }
        
    QString getDescription() { return moduleDescription; }
    
    int getIndex(){ return index_; }
    
    // Get module name.
    QString getName(){ return moduleName; } 
    
    // Returns current 'editable' state; 
    bool isEditable() {return editable;}    
    
    // Returns current index
    int setIndex(void) { return index_; }
    
    // Returns current 'modified' state.
    bool isModified(void) { return modified; }
    
    bool isHiding(void) { return config_->isHiding(); }
    
    // Sets 'editable' state.
    void setEditable (bool b) { editable = b; }   
    
    // Sets 'module hiding' state.
    void setHiding (bool b);
    
    // Sets 'modified' state.
    void setModified(bool b) { modified = b; }
    
    // Sets index of module relative to widget ordering.
    void setIndex( int newIndex ) { index_ = newIndex; }
  
protected:
    
/*** Protected Functions ****************/    

    void save();
  
    void load();
        
    void setDescription( QString name ) { moduleDescription = name; }
    
    void setModuleName( QString name ) { moduleName = QString(name); }    
    
    void setConfigWidget( SyModuleConfig * cfg );
    
  
/*** Protected Data Members ****************/

    SyModuleConfig * config_;     // Module config ui.
    
    bool hiding;
    
    enum{
      HIDDEN_INDEX = -1,      
      ADDED_INDEX = 100       
    };
    int index_;

    QString moduleName;
    QString moduleDescription;
    
    QString moduleHeader;
    
    QIcon icon;
    
    bool editable;
    bool modified;    
    
};

#endif