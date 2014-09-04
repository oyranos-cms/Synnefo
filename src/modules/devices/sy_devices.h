 
#ifndef SY_DEVICES_H
#define SY_DEVICES_H

#include <QApplication>
#include <QSignalMapper>
#include <QWidget>
#include <QComboBox>
#include <QThread>            // sleep

#include "symodule.h"
#include "sy_devices_item.h"
#include "sy_devices_config.h"

#include <oyranos_devices.h>

#include <stdio.h>
#include <stdlib.h>

namespace Ui {
  class syDevicesWidget;
}

/*****************************************************************
          Synnefo Devices Module Class Definition                *
******************************************************************/

class SyDevicesModule : public SyModule
{
    Q_OBJECT
    
public:
    SyDevicesModule( QWidget * parent );
    ~SyDevicesModule();
    
private slots:
  
    // Hitting the "Show only device related ICC profiles" button.
    void updateDeviceItems( int state );
    
    // Hitting the device profile selection combo box.
    void changeDeviceItem( int state );

    // When user clicks on a device tree item.
    void changeDeviceItem( QTreeWidgetItem*, int );
    
    void installTaxiProfile();

    // obtain the Taxi DB finished event
    void getTaxiSlot( char * for_device, oyConfigs_s * taxi_devices );

    // get the ICC profile
    void downloadFromTaxiDB( );

private:
    
    // Set new profile and update UI
    void setProfile( QString baseFileName );

    // get the actual device from currentDevice
    oyConfig_s * getCurrentDevice( void );
    
    // General device detection 
    int detectDevices( const char * device_type );
    
    // set the new profile to a Oyranos device
    void assignProfile( QString profile_name );
    
    // Populate device-specified profile combo box listing.
    void populateDeviceComboBox( QComboBox & itemComboBox, icProfileClassSignature deviceSignature, bool new_device );

    // Populate single device profile combo box widget
    void updateProfileList( QTreeWidgetItem * deviceItem, bool new_device );

    // Function to detect all devices/directories.
    void populateDeviceListing();

    // Update the Profile list.
    void updateLocalProfileList(QTreeWidgetItem * selected_device, bool new_device);

    // Convert profile filename into profile description (using Oyranos).
    QString convertFilenameToDescription( QString profileFilename );
    
    int checkProfileUpdates(oyConfig_s * device);
    
    QString getTaxiString(oyConfig_s * device, const char * oy_taxi_string);
    
    QString downloadTaxiProfile(oyConfig_s * device);
    int installTaxiProfile(oyConfig_s * config);
 
    // Download profile from Taxi server (based on device).
    QString checkRecentTaxiProfile(oyConfig_s * device);
    
  //***** Private Data Members.*********************
 

    // String used when user wants to add a new profile...
    QString recentlyAddedProfile;

    // Directory name variables.
    QString profileDirectoryDefault;

public:
    enum {
      ITEM_MAIN,        // SyDevicesItem column '0' -- Main display area.
      ITEM_COMBOBOX     // SyDevicesItem column '1' -- 'Profile Set' combobox.
    };
private:
    // Pointer used to store address of 'recently clicked' device item widget.
    SyDevicesConfig * devicesConfig;

    // Pointer used to store address of 'initial' device item widget.
    QTreeWidgetItem * deviceListPointer;

    // Pointer used to store address of 'recently clicked' device item widget.
    QTreeWidgetItem * currentDevice;

    bool listModified;                // Was the list changed by the user?
    bool init;                        // Are we inside tor?

    int icc_profile_flags;            // profile selection flags from oyProfile_s.h

    // Global string values for Oyranos device identification
    char * current_device_name;
    char * current_device_class;
    void setCurrentDeviceName(const char * name)
    { if(current_device_name) free(current_device_name);
      current_device_name = strdup(name); };
    void setCurrentDeviceClass(const char * name)
    { if(current_device_class) free(current_device_class);
      current_device_class = strdup(name); };

    Ui::syDevicesWidget * ui;
};


class TaxiLoad : public QThread
{
    Q_OBJECT

    oyConfig_s * d_;

        TaxiLoad( ) { d_ = 0; }
    public:
        TaxiLoad( oyConfig_s * device ) { d_ = device; }
        ~TaxiLoad( ) { }
     
    signals:
        void finishedSignal( char * device_name, oyConfigs_s * taxi_devices );
     
    protected:
        void run() {
            oyConfigs_s * taxi_devices = 0;
            char * device_name = 0;
            if(d_)
            {
              oyDevicesFromTaxiDB( d_, 0, &taxi_devices, 0);
              device_name = strdup(oyConfig_FindString( d_, "device_name", NULL ));
            }
            oyConfig_Release( &d_ );
            emit finishedSignal( device_name, taxi_devices );
        }
};

#endif
