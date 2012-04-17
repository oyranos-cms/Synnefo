#include <QMessageBox>
#include <QProgressDialog>
#include <QFile>

#include "sy_devices.h"
#include "sy_devices_config.h"

#include <oyranos.h>
#include <oyranos_icc.h>
#include <alpha/oyranos_alpha.h>
#include <alpha/oyranos_cmm.h>

// FIXME What would be the best path to automatically download profiles?
#define TAXI_DOWNLOAD_PATH "/tmp/"

const char * sy_devices_module_name = "Devices";

SyDevices::SyDevices(QWidget * parent)
    : SyModule(parent)
{         
    setModuleName(sy_devices_module_name);
    setDescription("Set profiles for the devices on your system.");
    
    this->setParent(parent);
    
    devicesConfig = new SyDevicesConfig(0, sy_devices_module_name);
    setConfigWidget( devicesConfig );
    
    setEditable(true);
    
    current_device_name = 0;
    current_device_class = 0;

    listModified = false;       // avoid action on signals
    
    setupUi(this);
    
    deviceList->setMouseTracking(true);
    
    // Disable for now.
    // NOTE Will eventually be removed from base widget.
    profileAssociationGroupBox->setVisible(false);
    
    // Disable all buttons
    deviceProfileComboBox->setEnabled(false); 
    
    // Set column width of device list.
    deviceList->setColumnWidth(0, 400);
    
    // Load directories and device listing.
    populateDeviceListing();
    
    // Expand list for user.
    deviceList->expandAll();
    
    connect( deviceList, SIGNAL(itemClicked( QTreeWidgetItem*, int)),
             this, SLOT( changeDeviceItem( QTreeWidgetItem*)) );
    connect( relatedDeviceCheckBox, SIGNAL(stateChanged( int )),
             this, SLOT( changeDeviceItem( int )) );
    connect( deviceProfileComboBox, SIGNAL(activated(int)),
             this, SLOT( openProfile(int)) );
    connect( profileAssociationList, SIGNAL( itemDoubleClicked( QListWidgetItem* )),
             this, SLOT( profileListDoubleClicked( QListWidgetItem * )) );
    connect( deviceList, SIGNAL( itemEntered( QTreeWidgetItem*, int )),
             this, SLOT( showProfileCombobox( QTreeWidgetItem* , int )));
}

// small helper to obtain a profile from a device
int syDeviceGetProfile( oyConfig_s * device, oyProfile_s ** profile )
{
    oyOptions_s * options = 0;
    oyOptions_SetFromText( &options,
                     "//"OY_TYPE_STD"/config/icc_profile.x_color_region_target",
                           "yes", OY_CREATE_NEW );
    int error = oyDeviceGetProfile( device, options, profile );
    oyOptions_Release( &options );
    return error;
}


void SyDevices::updateProfileCombo( QTreeWidgetItem * deviceItem )
{
  QTreeWidgetItem* device_class_item = deviceItem->parent();
  QVariant v = device_class_item->data( 0, Qt::UserRole );
  QString qs_device_class = v.toString();
  QByteArray raw_string = qs_device_class.toLatin1();
  char * device_class = strdup(raw_string.data());
  SyDevicesItem * device_data = dynamic_cast<SyDevicesItem*>(deviceItem);
  raw_string = device_data->getText(DEVICE_NAME).toLatin1();
  const char * device_name = strdup(raw_string.data());
  // Generate profiles in the combobox for current item.
  oyConfDomain_s * d = oyConfDomain_FromReg( device_class, 0 );
  const char * icc_profile_class = oyConfDomain_GetText( d,
                                                 "icc_profile_class", oyNAME_NICK );
  QWidget * w = deviceList->itemWidget(deviceItem, SyDevices::ITEM_COMBOBOX);
  QLayout * layout = w->layout();
  QComboBox * profileAssociationCB = dynamic_cast<QComboBox*> (layout->itemAt(0)->widget());

  setCurrentDeviceClass(device_class);
  setCurrentDeviceName(device_name);

  if(icc_profile_class && strcmp(icc_profile_class,"display") == 0)
    populateDeviceComboBox(*profileAssociationCB, icSigDisplayClass);
  else if(icc_profile_class && strcmp(icc_profile_class,"output") == 0)
    populateDeviceComboBox(*profileAssociationCB, icSigOutputClass);
  else if(icc_profile_class && strcmp(icc_profile_class,"input") == 0)
    populateDeviceComboBox(*profileAssociationCB, icSigInputClass);

  oyConfDomain_Release( &d );

  qWarning( "deviceList: %s - %s", device_class, device_name );
}

//  ******** SIGNAL/SLOT Functions *****************

void SyDevices::changeDeviceItem(int /*state*/)
{     
    for(int i = 0; i < deviceList->topLevelItemCount(); ++i)
    {
      QTreeWidgetItem* device_class_item = deviceList->topLevelItem(i);
      for(int j = 0; j < device_class_item->childCount(); ++j)
      {
        QTreeWidgetItem * deviceItem = device_class_item->child(j);
        updateProfileCombo( deviceItem );
        qWarning( "deviceList: [%d][%d]", i,j );
      }
    }

    //changeDeviceItem( currentDevice );
} 

// NOTE Dynamic item information (for each item click) update might be removed.
// When the user clicks on an item in the devices tree list.
void SyDevices::changeDeviceItem(QTreeWidgetItem * selectedDeviceItem)
{  
    if(!selectedDeviceItem)
    {
      deviceProfileComboBox->clear();
      deviceProfileComboBox->setEnabled(false);
      return;
    }

    // Don't count top parent items as a "selected device".
    if (selectedDeviceItem->parent() == NULL)
    {
         deviceProfileComboBox->setEnabled(false);
        
         return;
    }

    // The user modifies the list, but clicks away from the selected device item.
    listModified = false;

    // If we click on a device item, the current device is stored and options are available.
    deviceProfileComboBox->setEnabled(true);

    currentDevice = selectedDeviceItem;
 
    // Convert QString to proper C string.
    QByteArray raw_string;
    raw_string = (currentDevice->text(DEVICE_NAME)).toLatin1();
    setCurrentDeviceName(raw_string.data());
        
    char * device_class = 0;
    if(selectedDeviceItem && selectedDeviceItem->parent())
    {
      QVariant v = selectedDeviceItem->parent()->data( 0, Qt::UserRole );
      QString qs_device_class = v.toString();
      QByteArray raw_string;
      raw_string = qs_device_class.toLatin1();
      device_class = strdup(raw_string.data());
    }

    // Change "Available Device Profiles" combobox to device-related profiles.
    if ( device_class )
    {  
      oyConfDomain_s * d = oyConfDomain_FromReg( device_class, 0 );
      const char * icc_profile_class = oyConfDomain_GetText( d,
                                             "icc_profile_class", oyNAME_NICK );
      setCurrentDeviceClass(device_class);

      oyConfDomain_Release( &d );
      free(device_class); device_class = 0;
    }

    // Get the device that the user selected.
    oyConfig_s * device = 0;
    device = getCurrentDevice();

    updateProfileList(device); 
    oyConfig_Release(&device);
}


// Add a new profile to the list.
// NOTE Removed until appropriate implementation is available.
void SyDevices::openProfile(int /*index*/)
{
  #if 1
    int parenthesis_index = 0, base_filename_index = 0, str_size = 0, i;        
    QString baseFileName = deviceProfileComboBox->currentText(),
            tempProfile;

    QListWidgetItem * temp_item = new QListWidgetItem;

    QString description = baseFileName;    
    parenthesis_index = baseFileName.indexOf("\t(");   

    // Clean-up full text in the deviceComboBox (we will o>setText(2)nly use the file name to
    //                                          add to the profiles list).
    str_size = baseFileName.size();
    baseFileName.remove(0, parenthesis_index + 2);
    baseFileName.remove(str_size - 2, 1);
    parenthesis_index = baseFileName.indexOf(")");
    baseFileName.remove(parenthesis_index, parenthesis_index + 1);
    base_filename_index = baseFileName.lastIndexOf("/");
    baseFileName.remove(0, base_filename_index + 1); 

    // Error if user adds a duplicate profile.
    for(i = 0; i < profileAssociationList->count(); i++)
    {
          temp_item = profileAssociationList->item(i);
          if(temp_item->text() == baseFileName)
          {
	          // TODO Return some message here.
                  return;
          }
    }
    
    listModified = true;

    assignProfile( baseFileName );

    // Convert QString to proper C string.
    QByteArray raw_string;
    raw_string = (currentDevice->text(DEVICE_NAME)).toLatin1();
    setCurrentDeviceName(raw_string.data());
        
    // Get the device that the user selected.
    oyConfig_s * device = 0;     
    device = getCurrentDevice();
 
    updateProfileList(device); 
    oyConfig_Release(&device);
    
  #endif
}


void SyDevices::profileListDoubleClicked( QListWidgetItem * item )
{
    int i,j,n, k,k_n;
    oyConfigs_s * db_list = 0,
                * matches = 0;
    oyConfig_s * config = 0;
    const char * pattern[][2] = {{"device_name",0},
                                 {"manufacturer",0},
                                 {"model",0},
                                 {"serial",0},
                                 {0,0}};
    char * t;
    oyConfig_s * device = getCurrentDevice();
    oyOptions_s * options = 0;
    oyOption_s * o = 0;

    j = 0;
    while(pattern[j][0])
    {
      pattern[j][1] = oyConfig_FindString( device, pattern[j][0], 0);
      ++j;
    }

    oyConfigs_FromDB( device->registration, &db_list, 0 );
    oyConfigs_SelectSimiliars( db_list, pattern, &matches );

    n = profileAssociationList->count();
    for(i = 0; i < n; ++i)
    {
      if(profileAssociationList->item( i ) == item)
      {
        QString d;
        config = oyConfigs_Get( matches, i );
      
        k_n = oyConfig_Count( config );
        for(k = 0; k < k_n; ++k)
        {
          o = oyConfig_Get( config, k );
          t = oyOption_GetValueText( o, malloc );
          if(t)
          {
              d.append( strrchr( oyOption_GetRegistration(o), '/' ) + 1 );
              d.append(":\t");
              d.append( t );
              d.append("\n");
            free( t );
          }
        }

      //KMessageBox::information( this, i18n("Oyranos DB entry:\n") + d );

        oyConfig_Release( &config );
      }
    }

    oyConfig_Release( &device );
    oyConfigs_Release( &db_list );
    oyConfigs_Release( &matches );
    oyOptions_Release( &options );
}

// Make comboboxes visible only when mouse is over an item.
void SyDevices::showProfileCombobox( QTreeWidgetItem* item, int column)
{
  #if 0
   if (column == ITEM_MAIN)
     return;
   else if ( (column == ITEM_COMBOBOX) && (item->childCount() == 0) ) {
     QWidget * w = new QWidget();
     
     w = deviceList->itemWidget( item, column );
     w->setVisible(true);
     deviceList->setItemWidget( item, column, w );
   }   
   #endif
   
}

// ************** Private Functions ********************

// Helper to obtain device string ID.
QString getDeviceName(oyConfig_s * device)
{ 
    const char * manufacturer = oyConfig_FindString( device,"manufacturer",0);
    const char * model = oyConfig_FindString( device, "model", 0);
    const char * serial = oyConfig_FindString( device, "serial", 0); 
    return QString(manufacturer)+" "+QString(model)+" "+QString(serial);  
}

int SyDevices::installTaxiProfile(oyConfig_s * device)
{    
    int error = 0;
    QString taxiProfileName = downloadTaxiProfile(device);
   
    if (!taxiProfileName.isEmpty())
    {
      const char * profile_name = taxiProfileName.toLocal8Bit();
      char * pn = strdup(profile_name);

      // Install the profile.
      error = oyDeviceSetProfile(device, pn);
      error = oyDeviceUnset(device);
      error = oyDeviceSetup(device);
    }
    else
      error = 1;
    
    return error;
}

// Ping the Taxi server for recent profiles. 
int SyDevices::checkProfileUpdates(oyConfig_s * device)
{
    int is_installed = 0;
    
    QString taxiProfileDescription = checkRecentTaxiProfile(device);    

    if(!taxiProfileDescription.isEmpty())
    {
      int error, ret = 0;
      
      QMessageBox msgBox;
      msgBox.setText("A new profile is available to download for " 
                     + getDeviceName(device) + ".");
      msgBox.setInformativeText("Do you wish to install it?");
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgBox.setDefaultButton(QMessageBox::Yes);
      ret = msgBox.exec();

      switch(ret)
      {
         case QMessageBox::Yes:
         {
           error = installTaxiProfile(device);

	   if(!error) 
             is_installed = 1; 
           
           break;
         }
         case QMessageBox::No:
           break;
         default:
           break;  
      }
    }  
    
    return is_installed;
}

// General function to detect and retrieve devices via the Oyranos CMM backend.
int SyDevices::detectDevices(const char * device_type)
{ 
    int error = 0;
    oyConfigs_s * device_list = 0;
    oyOptions_s * options = oyOptions_New(0);
    oyConfDomain_s * d = oyConfDomain_FromReg( device_type, 0 );
    const char * reg_app = strrchr(device_type,'/')+1;
    const char * device_class = oyConfDomain_GetText( d, "device_class",
                                                      oyNAME_NICK );

    oyOptions_SetFromText(&options, "//" OY_TYPE_STD "/config/command", 
                          "properties", OY_CREATE_NEW);

    oyDevicesGet( OY_TYPE_STD, reg_app, 0, &device_list); 
        
    int j = 0, 
        device_num = oyConfigs_Count(device_list);

    // Must have at least one device detected to add to the list.
    if(device_num > 0)
    {
        // Set up Synnefo gui "logistics" for a specified device.
        QTreeWidgetItem * device_class_item = new QTreeWidgetItem;
        device_class_item->setText( ITEM_MAIN, oyConfDomain_GetText( d, "device_class",
                                                      oyNAME_NAME ));
        QVariant v( device_class );
        device_class_item->setData( 0, Qt::UserRole, v );
        deviceList->insertTopLevelItem( ITEM_MAIN, device_class_item );

        QIcon device_icon;
        QSize icon_size(64, 64);
        QString iconPath = QString(":/resources/") + device_class + ".png";
        device_icon.addFile( iconPath.toLower(), icon_size , QIcon::Normal, QIcon::On);

        QString taxiProfileDescription = "";
        // Traverse through the available devices 
        for (j = 0; j < device_num; j++)
        {
            QString deviceItemString, deviceProfileDescription;

            oyConfig_s * device = oyConfigs_Get(device_list, j);
            error = oyDeviceBackendCall(device, options);
 
            const char * device_manufacturer = 0;
            const char * device_model = 0;
            const char * device_serial = 0;
            char * device_designation = 0;

            const char * profile_filename = 0;
            oyProfile_s * profile = 0;

            device_manufacturer = oyConfig_FindString( device,"manufacturer",0);
            device_model = oyConfig_FindString( device, "model", 0);
            device_serial = oyConfig_FindString( device, "serial", 0);
    
            
            // Get device designation.
            error = oyDeviceGetInfo(device, oyNAME_NICK, 0, &device_designation, malloc);
            setCurrentDeviceName(device_designation);
            setCurrentDeviceClass(device_class);
 
            // A printer will only take a "device model"
            if (strcmp(device_class,"printer") != 0)
            {
                deviceItemString.append(device_manufacturer);
                deviceItemString.append(" ");
            }

            deviceItemString.append(device_model);
            if(device_serial)
            {
              deviceItemString.append(" ");
              deviceItemString.append(device_serial);
            }
            
            // TESTING Check device for Taxi updates.
            if (strcmp(device_class, "monitor") == 0)
              checkProfileUpdates(device);

            error = syDeviceGetProfile(device, &profile);
            profile_filename = oyProfile_GetFileName(profile, 0);
 
            SyDevicesItem * deviceItem = new SyDevicesItem(0);

            if (profile_filename == NULL)
            {
                deviceProfileDescription = oyProfile_GetText( profile, oyNAME_DESCRIPTION );
                if(!deviceProfileDescription.count())
                  deviceProfileDescription = "(No Profile Installed!)";
                profile_filename = "------";
            }
            else
                deviceProfileDescription = convertFilenameToDescription(profile_filename);

            deviceItem->setIcon( ITEM_MAIN, device_icon );
            
            deviceItem->addText(DEVICE_DESCRIPTION, device_model);
            deviceItem->addText(DEVICE_NAME, device_designation);
            deviceItem->addText(PROFILE_DESCRIPTION, deviceProfileDescription);   
            deviceItem->addText(PROFILE_FILENAME, profile_filename);

            deviceItem->refreshText();
    
            // NOTE: New code to add association combo box in tree widget.
            QComboBox * profileAssociationCB = new QComboBox();
   
            QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
            layout->addWidget (profileAssociationCB);

            QWidget * w = new QWidget();
            w->setLayout(layout);
 
            device_class_item->addChild( deviceItem );
            deviceList->setItemWidget( deviceItem, ITEM_COMBOBOX, w);

            updateProfileCombo( deviceItem );

            oyConfig_Release(&device);
        }
     }
     else
        error = -1;

     oyOptions_Release ( &options );
     oyConfigs_Release ( &device_list );
     oyConfDomain_Release( &d );
 
     return error;
}

// Populate devices and profiles.
void SyDevices::populateDeviceListing()
{
    // TODO Work out a solution to use raw/camera stuff.
    // detectRaw();  

    uint32_t count = 0, i = 1,
           * rank_list = 0;
    char ** texts = 0;
    int error = 0;

    // get all configuration filters
    oyConfigDomainList( "//"OY_TYPE_STD"/config.device.icc_profile",
                        &texts, &count, &rank_list ,0 );

    for (i = 0; i < count; i++)
    {
      error = detectDevices( texts[i] );
    }
}


// Update profile association list every time a user clicks on a device item.
void SyDevices::updateProfileList(oyConfig_s * device)
{
    int i, j;
    oyProfile_s * profile = 0;
    const char * profile_name = 0;
    oyConfigs_s * db_list = 0,
                * matches = 0;
    oyConfig_s * config;
    const char * pattern[][2] = {{"device_name",0},
                             {"manufacturer",0},
                             {"model",0},
                             {"serial",0},
                             {0,0}};

    if(!device)
    {
      changeDeviceItem((QTreeWidgetItem *)NULL);
      return;
    }

    profileAssociationList->clear();

    syDeviceGetProfile( device, &profile );
    profile_name = oyProfile_GetText(profile, oyNAME_DESCRIPTION);

    j = 0;
    while(pattern[j][0])
    {
      pattern[j][1] = oyConfig_FindString( device, pattern[j][0], 0);

      ++j;
    }

    oyConfigs_FromDB( device->registration, &db_list, 0 );
    oyConfigs_SelectSimiliars( db_list, pattern, &matches );

    int n = oyConfigs_Count( matches );
    for(i = 0; i < n; ++i)
    {
      config = oyConfigs_Get( matches, i );
      
      profile_name = oyConfig_FindString( config, "profile_name", 0);
      if( profile_name )
        profileAssociationList->addItem( profile_name );

      oyConfig_Release( &config );
    }

    oyConfigs_Release( &matches );
    oyConfigs_Release( &db_list );
}

// Populate "Assign Profile" combobox.  Depending on the device selected, the profile list will vary.
void SyDevices::populateDeviceComboBox( QComboBox & itemComboBox, icProfileClassSignature deviceSignature )
{
    int size, i, current = -1, current_tmp, pos = 0;
    oyProfile_s * profile = 0, * temp_profile = 0;
    oyProfiles_s * patterns = 0, * iccs = 0;
    oyConfig_s * device = getCurrentDevice();
    const char * profile_file_name = 0;

    profile = oyProfile_FromSignature( deviceSignature, oySIGNATURE_CLASS, 0 );
    patterns = oyProfiles_MoveIn( patterns, &profile, -1 );

    iccs = oyProfiles_Create( patterns, 0 );
    oyProfiles_Release( &patterns );
 
    QString getProfileDescription;

    size = oyProfiles_Count(iccs);
    int32_t * rank_list = (int32_t*) malloc( oyProfiles_Count(iccs) *
                                             sizeof(int32_t) );
    oyProfiles_DeviceRank( iccs, device, rank_list );

    size = oyProfiles_Count(iccs);

    itemComboBox.clear();

    syDeviceGetProfile( device, &profile ); /* reget profile */
    profile_file_name = oyProfile_GetFileName( profile, 0 );

    Qt::CheckState show_only_device_related = 
                                          relatedDeviceCheckBox->checkState();
    int empty_added = -1;

    for( i = 0; i < size; ++i)
    {
      const char * temp_profile_file_name;
         temp_profile = oyProfiles_Get( iccs, i );
         getProfileDescription = oyProfile_GetText( temp_profile, oyNAME_DESCRIPTION );
         temp_profile_file_name = oyProfile_GetFileName( temp_profile, 0);
 
         current_tmp = -1;

         if(profile_file_name && temp_profile_file_name &&
            strcmp( profile_file_name, temp_profile_file_name ) == 0)
           current_tmp = pos;

         if(current == -1 && current_tmp != -1)
           current = current_tmp;

         if(empty_added == -1 &&
            rank_list[i] < 1)
         {
           itemComboBox.addItem("");
           empty_added = pos;
           if(current != -1 &&
              current == pos)
             ++current;
           ++pos;
         }

         if(show_only_device_related == Qt::Unchecked ||
            rank_list[i] > 0 ||
            current_tmp != -1)
         {
           getProfileDescription.append("\t(");
           getProfileDescription.append(temp_profile_file_name);
           getProfileDescription.append(")");
           itemComboBox.addItem(getProfileDescription);
           ++pos;
         }
      oyProfile_Release( &temp_profile );
    }
    
    if(empty_added == -1)
    {
      itemComboBox.addItem("");
      ++pos;
      if(current == -1 && current_tmp != -1)
        current = pos;
    }
    
    if(current == -1 && profile_file_name)
    {
      getProfileDescription = oyProfile_GetText( profile, oyNAME_DESCRIPTION );
      getProfileDescription.append("\t(");
      getProfileDescription.append(profile_file_name);
      getProfileDescription.append(")");
      itemComboBox.addItem(getProfileDescription);
      current = pos;
    }

    itemComboBox.setCurrentIndex( current );

    oyConfig_Release( &device );
    oyProfile_Release( &profile );
    oyProfiles_Release( &iccs );
} 

oyConfig_s * SyDevices::getCurrentDevice( void )
{
    oyConfig_s * device = 0;
    int error= 0;

    oyOptions_s * options = 0;
    oyOptions_SetFromText( &options, "//" OY_TYPE_STD "/config/command", 
                           "properties", OY_CREATE_NEW );
    oyOptions_SetFromText( &options,
                     "//"OY_TYPE_STD"/config/icc_profile.x_color_region_target",
                           "yes", OY_CREATE_NEW );
    if(current_device_class && current_device_name)
      error = oyDeviceGet( OY_TYPE_STD, current_device_class, current_device_name,
                           options, &device );
    Q_UNUSED(error);
 
    /* clear */
    oyOptions_Release( &options );

    return device;
}

#include <QThread>
class kmSleep : public QThread
{
  public:
     static void sleep(double seconds)
     { QThread::msleep((long unsigned int)(seconds*1000)); }
};

void SyDevices::assignProfile( QString & profile_name )
{        
     oyProfile_s * profile;
     QString description;

     // If current device pointer points to a MONITOR item, save default profile to Oyranos.

     {
         int error= 0;
         oyConfig_s * device = getCurrentDevice();
         const char * profilename = profile_name.toLocal8Bit();
         char * pn = strdup(profilename);

         /* store a existing profile in DB */
         if(strlen(pn))
           error = oyDeviceSetProfile ( device, pn );
         error = oyDeviceUnset( device ); /* unset the device */
         /* completly unset the actual profile from DB */
         if(!strlen(pn))
         {
           error = oyConfig_EraseFromDB( device );
           oyConfig_Release( &device );
           /* reopen the device to forget about the "profile_name" key */
           device = getCurrentDevice();
         }
         error = oyDeviceSetup( device ); /* reinitialise */
         /* compiz needs some time to exchange the profiles,
            immediately we would get the old colour server profile */
         kmSleep::sleep(0.3);
         error = syDeviceGetProfile( device, &profile ); /* reget profile */

         /* clear */
         oyConfig_Release( &device );
         if(pn) free(pn); pn = 0;
     }

     // Convert profile into description name...
     description = oyProfile_GetText( profile, oyNAME_DESCRIPTION);

     if(!description.count())
       description = "(No Profile Installed!)";

     // Set third column of the device list with the profile description.
     currentDevice->setText(PROFILE_DESCRIPTION, description);
 
     if(!profile_name.count())
       profile_name = "------";
     currentDevice->setText(PROFILE_FILENAME, profile_name);
}


/*                  
           ************* Oyranos Taxi Code *************
*/

// TESTING Code to verify if recent Taxi profile is already installed.
int isRecentProfile(oyConfig_s * device, oyConfig_s * taxi_device)
{
    int error, is_recent = 0;   
    char * current_profile = 0; 
    oyProfile_s * p1 = 0;
    oyProfile_s * p2 = 0;
    oyOptions_s * options = 0;    
    
    oyDeviceProfileFromDB(device, &current_profile, malloc);
    QString taxiIdString = QString(oyConfig_FindString(taxi_device, "TAXI_id",0)) + "/0";

    error = oyOptions_SetFromText(&options,
                                 "//" OY_TYPE_STD "/argv/TAXI_id",
                                 taxiIdString.toLocal8Bit(),
                                 OY_CREATE_NEW);
    if(!error)
    {
      p1 = oyProfile_FromTaxiDB(options, NULL);        
      p2 = oyProfile_FromFile(current_profile, 0, 0);
    
      if(oyProfile_Equal(p1, p2))
        is_recent = 1;
    
      oyProfile_Release(&p1);
      oyProfile_Release(&p2);
    
      free(current_profile);
      current_profile = 0;
    }
    
    oyOptions_Release(&options);
    
    return is_recent;    
}



int   compareRanks                   ( const void       * rank1,
                                       const void       * rank2 )
{int32_t *r1=(int32_t*)rank1, *r2=(int32_t*)rank2; if(r1[1] < r2[1]) return 1; else return 0;}

// Helper function to get 'best-ranking' profile from the Taxi server. 
oyConfig_s * getTaxiBestFit(oyConfig_s * device)
{
    int error = 0, n, i = 0;
    int32_t * ranks;
    oyConfig_s * taxi_dev;
    oyConfigs_s * devices = 0;
    oyOptions_s * options = NULL;
    
    error = oyOptions_SetFromText(&options,
                                  "//" OY_TYPE_STD "/config/command",
                                  "properties", OY_CREATE_NEW);

    oyDevicesFromTaxiDB(device, options, &devices, NULL);
    
    n = oyConfigs_Count(devices);
    
    if (n)
    {
      ranks = new int32_t[n*2+1];
      for(i = 0; i < n; ++i)
      {
        taxi_dev = oyConfigs_Get(devices, i);
        ranks[2*i+0] = i;
        error = oyConfig_Compare(device, taxi_dev, &ranks[2*i+1]);

        oyConfig_Release(&taxi_dev);
      }
      
      // TODO Fix ranking so that it uses C++ sorting algorithm instead.
      qsort(ranks, n, sizeof(int32_t)*2, compareRanks);      
      
      // Obtain the best-ranked device profile (0).
      taxi_dev = oyConfigs_Get(devices, 0);

      delete[] ranks;    
    }
        
    // Verify if we already have the Taxi profile installed.
    if(isRecentProfile(device, taxi_dev))
    {
       oyConfig_Release(&taxi_dev);
       taxi_dev = 0;
    }
    
    oyConfigs_Release(&devices);
    oyOptions_Release(&options);  
    
    return taxi_dev;
}

// Obtain either profile description or the taxi_id of a Taxi device (Server).
QString SyDevices::getTaxiString(oyConfig_s * device, const char * oy_taxi_string)
{
    QString str = "";
    oyConfig_s * taxi_dev = 0;
    
    taxi_dev = getTaxiBestFit(device);
    
    if(taxi_dev)
    {
      str = oyConfig_FindString(taxi_dev, oy_taxi_string, 0);    
      oyConfig_Release(&taxi_dev);
    }
    
    return str;
}

// Obtain the profile description of the "best fit" profile from Taxi.
QString SyDevices::checkRecentTaxiProfile(oyConfig_s * device)
{    
    QString profileDescription = getTaxiString(device, "TAXI_profile_description");     
    return profileDescription;
}

// Download a profile from Taxi server and return its filename.
QString SyDevices::downloadTaxiProfile(oyConfig_s * device)
{  
    QString fileName = "";
    int error, i = 0;
    oyProfile_s * ip = 0;    
    oyOptions_s * options = NULL;
    const char * file_name = 0;
    size_t size = 0;
    char * data = 0;
    
    QString taxiIdString = QString( getTaxiString(device, "TAXI_id") + "/0" );
    
    error = oyOptions_SetFromText(&options,
                                 "//" OY_TYPE_STD "/argv/TAXI_id",
                                 taxiIdString.toLocal8Bit(),
                                 OY_CREATE_NEW);

    // Download the taxi profile.
    if(!error)
    {
      ip = oyProfile_FromTaxiDB(options, NULL);  
      
      const char * taxi_profile_name = oyProfile_GetText( ip, oyNAME_DESCRIPTION );
      fileName = TAXI_DOWNLOAD_PATH + QString(taxi_profile_name) + ".icc";
      
      data = (char*)oyProfile_GetMem(ip, &size, 0, malloc);
            
      if(data)
      {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
      
        QDataStream out(&file);      
        out.writeRawData(data, size);
      
        file.close();
      
        oyProfile_Release(&ip);
        free(data);
      }
    }    
    
    oyOptions_Release(&options);
    
    return fileName;
}



/*                  
           ************* Oyranos DB Settings *************
*/

// This is so we can obtain a profile name description from a profile file name.
QString SyDevices::convertFilenameToDescription(QString profileFilename)
{    
    QString profileDescriptionName;
    oyProfile_s * profile_name;
    
    profile_name = oyProfile_FromFile( profileFilename.toLocal8Bit(), 0, 0);
    profileDescriptionName = oyProfile_GetText( profile_name, oyNAME_DESCRIPTION );

    return profileDescriptionName;
}


SyDevices::~SyDevices()
{
  
}
