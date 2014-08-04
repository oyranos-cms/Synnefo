 
#ifndef SY_INFO_H
#define SY_INFO_H

#include <QApplication>
#include <QSignalMapper>
#include <QWidget>
#include <QTemporaryFile>

#include <oyranos_devices.h>

#include "symodule.h"          
#include "sy_info_dialog.h"

// Qt Designer code translation.
#include "ui_sy_info.h"          

class SyInfo : public SyModule, Ui::syInfoWidget
{
    Q_OBJECT
    
public:
    SyInfo (QWidget * parent);
    ~SyInfo();   
    
    
// User-defined QT slots.
private slots:
    
    void profileExamineButtonClicked(QTreeWidgetItem *, int);
    void loadProfileGraph();
private:
  
  /*** PRIVATE FUNCTIONS ***/  
  
    // Build profile listing tree.
    void populateInstalledProfileList();
    
    // Add an item to the tree.
    void addProfileTreeItem( oyPROFILE_e, QString description, QTreeWidgetItem * parent_item );
    
    // Populate tree list items relating to device-specific profiles (printers, monitors, etc.)
    void populateDeviceProfiles
         ( /*QStringList listOfDevices,*/ QTreeWidgetItem * listTree /*, QIcon device_icon*/);

    // Populate tag descriptions for device-specific profiles.
    void populateDeviceProfileDescriptions(oyProfile_s * profile, bool valid);
    
    // Function to write tag descriptions to individual labels
    void setTagDescriptions(oyProfile_s *, icTagSignature, DialogString);
    
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
};

#endif
