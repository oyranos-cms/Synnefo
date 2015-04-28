 
#ifndef SY_INFO_H
#define SY_INFO_H

#include <QApplication>
#include <QSignalMapper>
#include <QWidget>
#include <QTemporaryFile>
#include <QTreeWidgetItem>

#include "symodule.h"          
#include "sy_info_dialog.h"

namespace Ui {
  class syInfoWidget;
}

struct oyProfile_s;

class SyInfoModule : public SyModule
{
    Q_OBJECT
    
public:
    SyInfoModule (QWidget * parent);
    ~SyInfoModule();   
    
    
// User-defined QT slots.
private slots:
    
    void profileExamineButtonClicked(QTreeWidgetItem *, int);
    void loadProfileGraph();
    // Build profile listing tree.
    void populateInstalledProfileList() { populateInstalledProfileList(false); };
    
private:
  
  /*** PRIVATE FUNCTIONS ***/  
    void populateInstalledProfileList(bool init);
  
    // Add an item to the tree.
    void addProfileTreeItem( unsigned int profile_type, QString description, QTreeWidgetItem * parent_item );
    
    // Populate tree list items relating to device-specific profiles (printers, monitors, etc.)
    void populateDeviceProfiles
         ( /*QStringList listOfDevices,*/ QTreeWidgetItem * listTree /*, QIcon device_icon*/);

    // Populate tag descriptions for device-specific profiles.
    void populateDeviceProfileDescriptions(oyProfile_s * profile, bool valid);
    
    // Function to write tag descriptions to individual labels
    void setTagDescriptions(oyProfile_s *, unsigned int, DialogString);
    
    // The following provide additonal tags to be displayed.
    void setPcsTag(oyProfile_s * profile);
    void setColorSpaceTag(oyProfile_s * profile);
    void setIccsTag(oyProfile_s * profile);
    void setDeviceClassTag(oyProfile_s * profile);
    
    // Function to write date tag to a label.
    void setDateTag(oyProfile_s *);
            
  /*** PRIVATE DATA MEMBERS ***/  
  
    SyInfoDialog * infoDialog;
    QIcon examineIcon;
    
    QFont boldFont;
    QFont normalFont;
      
    // Pointers to current QTreeWidget parents (Devices, Editing Space, Assumed Space)
    enum {
      ITEM_NAME,
      ITEM_DESCRIPTION,
      ITEM_ICON      
    };

    // Pointers to device-specific (parent) items on list
    oyProfile_s * current_profile;
          
    QTemporaryFile m_tempFile;

    int icc_profile_flags;              ///< profile selection flags from oyProfile_s.h

    Ui::syInfoWidget * ui;
};

#endif
