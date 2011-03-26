 
#ifndef SY_DEVICES_H
#define SY_DEVICES_H

#include <QApplication>
#include <QSignalMapper>
#include <QWidget>

#include "symodule.h"
#include "sy_devices_item.h"

// Qt Designer code translation.
#include "ui_sy_devices.h"          

#include <alpha/oyranos_alpha.h>
#include <alpha/oyranos_cmm.h>



/*****************************************************************
          Synnefo Devices Module Class Definition                *
******************************************************************/

class SyDevices : public SyModule, Ui::syDevicesWidget
{
    Q_OBJECT
    
public:
    SyDevices( QWidget * parent );
    ~SyDevices();
    
private slots:
  
    // When the "Assign Profile" button is pressed...
    void openProfile(int index);
    
    // Hitting the "Show only device related ICC profiles" button.
    void changeDeviceItem( int state );
    
    // When user clicks on a device tree item.
    void changeDeviceItem( QTreeWidgetItem* );
    
    // When user clicks on a profile list item
    void profileListDoubleClicked( QListWidgetItem* );
    
private:
    
    // get the actual device from currentDevice
    oyConfig_s * getCurrentDevice( void );
    
    // General device detection 
    int detectDevices( const char * device_type );
    
    // set the new profile to a Oyranos device
    void assignProfile( QString & profile_name );
    
    // Populate device-specified profile combo box listing.
    void populateDeviceComboBox( icProfileClassSignature deviceSignature );
    
    // Function to detect all devices/directories.
    void populateDeviceListing();
    
    // Refresh current profile list.
    void updateProfileList(oyConfig_s * device);
    
    // Convert profile filename into profile description (using Oyranos).
    QString convertFilenameToDescription( QString profileFilename );
    
    
  //***** Private Data Members.*********************
 
    
    // String used when user wants to add a new profile...
    QString recentlyAddedProfile;
    
    // Directory name variables.
    QString profileDirectoryDefault;
    
    //QTreeWidgetItem * deviceListPointer;
    SyDevicesItem * deviceListPointer;
    
    // Pointer used to store address of 'recently clicked' device item widget.
    QTreeWidgetItem * currentDevice;

    bool listModified;                // Was the list changed by the user?
    
    // Global string values for Oyranos device identification 
    char * current_device_name;
    char * current_device_class;  
    void setCurrentDeviceName(const char * name)
    { if(current_device_name) free(current_device_name);
      current_device_name = strdup(name); };
    void setCurrentDeviceClass(const char * name)
    { if(current_device_name) free(current_device_class);
      current_device_class = strdup(name); };
};


#endif