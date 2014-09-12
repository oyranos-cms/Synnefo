#include <QMessageBox>
#include <QProgressDialog>
#include <QFile>
#include <QTimer>

#include "sy_devices.h"
#include "sy_devices_config.h"

// Qt Designer code translation.
#include "ui_sy_devices.h"          

#include <oyranos.h>
#include <oyranos_devices.h>
#include <oyFilterNode_s.h>
#include <oyObject_s.h>
#include <oyProfiles_s.h>

#define i18n(t) QString(t)

// Use the users personal ICC path for installation.
#define TAXI_DOWNLOAD_PATH OY_USERCOLORDATA OY_SLASH OY_ICCDIRNAME

const char * sy_devices_module_name = "Devices";

// a bag to provide a otherwise hard to obtain link to the parent widget
class SyDeviceItem : public QComboBox
{
  public:
    SyDeviceItem () : QComboBox() {}
    void setParent ( SyDevicesItem * p ) {parent_ = p;}
    SyDevicesItem * getParent () {return parent_;}
  private:    
    SyDevicesItem * parent_;
};

SyDevicesModule::SyDevicesModule(QWidget * parent)
    : SyModule(parent)
{         
    setModuleName(sy_devices_module_name);
    setDescription("Set profiles for the devices on your system.");
    
    const char    * name = NULL,
                  * description = NULL,
                  * tooltip = NULL;

    oyWidgetTitleGet( oyWIDGET_GROUP_DEVICES, NULL, &name,
                      NULL, NULL );
    oyWidgetDescriptionGet( oyWIDGET_GROUP_DEVICES, &description, 0 );
    setModuleName(QString::fromLocal8Bit(name));
    setDescription(QString::fromLocal8Bit(description));

    this->setParent(parent);
    
    devicesConfig = new SyDevicesConfig(0, sy_devices_module_name);
    setConfigWidget( devicesConfig );
    
    setEditable(true);
    
    currentDevice = 0;
    current_device_name = 0;
    current_device_class = 0;

    listModified = false;       // avoid action on signals
    init = true;

    // select profiles matching actual capabilities
    icc_profile_flags = oyICCProfileSelectionFlagsFromOptions( OY_CMM_STD, "//" OY_TYPE_STD "/icc_color", NULL, 0 );

    ui = new Ui::syDevicesWidget;
    ui->setupUi(this);
    
    ui->deviceList->setMouseTracking(true);

    // Set column width of device list.
    ui->deviceList->setColumnWidth(0, 400);

    /* i18n */
    QString qs;

    oyWidgetTitleGet( oyWIDGET_DEVICES_RELATED, NULL, &name, &tooltip, NULL );
    qs = QString::fromLocal8Bit(tooltip);
    ui->relatedDeviceCheckBox->setText(qs);
    oyWidgetDescriptionGet( oyWIDGET_DEVICES_RELATED, &description, 0 );
    qs = QString::fromLocal8Bit(description);
    ui->relatedDeviceCheckBox->setToolTip(qs);

    oyWidgetTitleGet( oyWIDGET_GROUP_DEVICES_PROFILES_TAXI, NULL, &name, &tooltip, NULL );
    qs = QString::fromLocal8Bit(tooltip);
    ui->taxiGroupBox->setTitle(qs);
    oyWidgetDescriptionGet( oyWIDGET_GROUP_DEVICES_PROFILES_TAXI, &description, 0 );
    qs = QString::fromLocal8Bit(description);
    ui->deviceProfileTaxiDBComboBox->setToolTip(qs);

    oyWidgetTitleGet( oyWIDGET_TAXI_PROFILE_INSTALL, NULL, &name, &tooltip, NULL );
    qs = QString::fromLocal8Bit(tooltip);
    ui->installProfileButton->setText(qs);
    // first select a device, then we can do something useful
    ui->installProfileButton->setEnabled(false);

    // Load directories and device listing.
    populateDeviceListing();
    
    // Expand list for user.
    ui->deviceList->expandAll();
    
    connect( ui->relatedDeviceCheckBox, SIGNAL(stateChanged( int )),
             this, SLOT( updateDeviceItems( int )) );
    connect( ui->deviceList, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
             this, SLOT(changeDeviceItem(QTreeWidgetItem*,int)) );
    connect( ui->installProfileButton, SIGNAL(clicked()),
	     this, SLOT(installTaxiProfile()));
    init = false;
}

// small helper to obtain a profile from a device
int syDeviceGetProfile( oyConfig_s * device, uint32_t icc_profile_flags, oyProfile_s ** profile )
{
    oyOptions_s * options = 0;
    oyOptions_SetFromText( &options,
                     "//"OY_TYPE_STD"/config/icc_profile.x_color_region_target",
                           "yes", OY_CREATE_NEW );
    oyOptions_SetFromInt( &options, "///icc_profile_flags", icc_profile_flags, 0, OY_CREATE_NEW );
    int error = oyDeviceGetProfile( device, options, profile );
    oyOptions_Release( &options );
    return error;
}


void SyDevicesModule::updateProfileList( QTreeWidgetItem * selected_device, bool new_device )
{
  QWidget * w = ui->deviceList->itemWidget(selected_device, SyDevicesModule::ITEM_COMBOBOX);
  if(!w) return;

  QLayout * layout = w->layout();
  QComboBox * profileAssociationCB = dynamic_cast<QComboBox*> (layout->itemAt(0)->widget());

    if(!selected_device)
    {
      ui->deviceProfileTaxiDBComboBox->clear();
      ui->deviceProfileTaxiDBComboBox->setEnabled(false);

      return;
    }

    // Don't count top parent items as a "selected device".
    QTreeWidgetItem * parent = selected_device->parent();
    if (parent == NULL)
    {
      ui->deviceProfileTaxiDBComboBox->setEnabled(false);

      return;
    }

    // The user modifies the list, but clicks away from the selected device item.
    listModified = false;

    // If we click on a device item, the current device is stored and options are available.
    ui->deviceProfileTaxiDBComboBox->setEnabled(true);

    currentDevice = selected_device;

    // Convert QString to proper C string.
    QByteArray raw_string;
    SyDevicesItem * device_item = dynamic_cast<SyDevicesItem*>(selected_device);
    raw_string = device_item->getText(DEVICE_NAME).toLocal8Bit();
    setCurrentDeviceName(raw_string.data());

    char * device_class = 0;
    if(selected_device && selected_device->parent())
    {
      QVariant v = selected_device->parent()->data( 0, Qt::UserRole );
      QString qs_device_class = v.toString();
      QByteArray raw_string;
      raw_string = qs_device_class.toLocal8Bit();
      device_class = strdup(raw_string.data());
    }

    // Change "Available Device Profiles" combobox to device-related profiles.
    if ( device_class )
    {
      oyConfDomain_s * d = oyConfDomain_FromReg( device_class, 0 );
      const char * icc_profile_class = oyConfDomain_GetText( d,
                                             "icc_profile_class", oyNAME_NICK );
      setCurrentDeviceClass(device_class);

      if(icc_profile_class && strcmp(icc_profile_class,"display") == 0)
        populateDeviceComboBox(*profileAssociationCB, icSigDisplayClass, new_device);
      else if(icc_profile_class && strcmp(icc_profile_class,"output") == 0)
        populateDeviceComboBox(*profileAssociationCB, icSigOutputClass, new_device);
      else if(icc_profile_class && strcmp(icc_profile_class,"input") == 0)
        populateDeviceComboBox(*profileAssociationCB, icSigInputClass, new_device);

      oyConfDomain_Release( &d );
      free(device_class); device_class = 0;
    }

}

//  ******** SIGNAL/SLOT Functions *****************

void SyDevicesModule::updateDeviceItems(int state)
{
    init = true;  // skip signals in changeDeviceItem
    for(int i = 0; i < ui->deviceList->topLevelItemCount(); ++i)
    {
      QTreeWidgetItem* device_class_item = ui->deviceList->topLevelItem(i);
      for(int j = 0; j < device_class_item->childCount(); ++j)
      {
        QTreeWidgetItem * deviceItem = device_class_item->child(j);
        updateProfileList( deviceItem, false );
        qWarning( "deviceList: [%d][%d]", i,j );
      }
    }
    init = false;
} 

void SyDevicesModule::changeDeviceItem(int pos)
{
  SyDeviceItem * combo = dynamic_cast<SyDeviceItem*>(sender());
  if(combo && !init && pos >= 0)
  {
    SyDevicesItem * device_item = combo->getParent();
    // unselect all tree items
    for(int i = 0; i < ui->deviceList->topLevelItemCount(); ++i)
    {
      QTreeWidgetItem* device_class_item = ui->deviceList->topLevelItem(i);
      for(int j = 0; j < device_class_item->childCount(); ++j)
      {
        QTreeWidgetItem * deviceItem = device_class_item->child(j);
        if(device_item != deviceItem && deviceItem->isSelected())
          deviceItem->setSelected(false);
      }
    }
    // select the belonging widget to this combobox
    //device_item->setSelected(true);

    // get infos
    QVariant v = device_item->parent()->data( 0, Qt::UserRole );
    QString qs = v.toString();
    QByteArray raw_string = qs.toLocal8Bit();
    char * device_class = strdup(raw_string.data());
    v = combo->itemData(pos, Qt::UserRole);
    qs = v.toString();
    raw_string = qs.toLocal8Bit();
    char * profile_name = strdup(raw_string.data());
    raw_string = device_item->getText(DEVICE_NAME).toLocal8Bit();
    char * device_name = raw_string.data();

    // set internal context
    setCurrentDeviceClass(device_class);
    setCurrentDeviceName(device_name);
    qWarning( "%d deviceItem: %d %s %s: %s", __LINE__,pos, device_class,
              device_item->getText(DEVICE_NAME).toLocal8Bit().data(),
              profile_name);

    // set profile
    assignProfile(QString(profile_name));
  }
} 

// NOTE Dynamic item information (for each item click) update might be removed.
// When the user clicks on an item in the devices tree list.
void SyDevicesModule::changeDeviceItem(QTreeWidgetItem * selected_device, int pos)
{
    ui->deviceProfileTaxiDBComboBox->clear();
    ui->installProfileButton->setEnabled(false);

    QWidget * w = ui->deviceList->itemWidget(selected_device, SyDevicesModule::ITEM_COMBOBOX);
    if(!w)
    {
      ui->deviceProfileTaxiDBComboBox->setEnabled(false);
      return;
    }

    currentDevice = selected_device;

    // Convert QString to proper C string.
    QByteArray raw_string;
    SyDevicesItem * device_item = dynamic_cast<SyDevicesItem*>(selected_device);
    raw_string = device_item->getText(DEVICE_NAME).toLocal8Bit();
    const char * t = raw_string.data();
    setCurrentDeviceName(t);

    char * device_class = 0;
    if(selected_device && selected_device->parent())
    {
      QVariant v = selected_device->parent()->data( 0, Qt::UserRole );
      QString qs_device_class = v.toString();
      QByteArray raw_string;
      raw_string = qs_device_class.toLocal8Bit();
      device_class = strdup(raw_string.data());
    }

    // Change "Available Device Profiles" combobox to device-related profiles.
    if ( device_class )
      setCurrentDeviceClass(device_class);

    // asynchronous Taxi DB query
    oyConfig_s * device = getCurrentDevice();
    if(device)
    {
      ui->deviceProfileTaxiDBComboBox->clear();

      // msgWidget->setMessageType(QMessageBox::Information);
      ui->msgWidget->setText(i18n("Looking for Device Profiles in Taxi DB ..."));

      TaxiLoad * loader = new TaxiLoad( oyConfig_Copy( device, oyObject_New() ) );
      connect(loader, SIGNAL(finishedSignal( char *, oyConfigs_s * )), this, SLOT( getTaxiSlot( char*, oyConfigs_s* )));
      loader->start();

      /* clear */
      oyConfig_Release( &device );
    } else
      ui->deviceProfileTaxiDBComboBox->setEnabled(false);
}

void SyDevicesModule::installTaxiProfile()
{
    // msgWidget->setMessageType(QMessageBox::Information);
    ui->msgWidget->setText(i18n("Downloading Profile from Taxi DB ..."));

    QTimer::singleShot(100, this, SLOT(downloadFromTaxiDB()));
}

void SyDevicesModule::downloadFromTaxiDB( )
{
    oyProfile_s * ip = 0;
    oyOptions_s * options = 0;
    char * id = (char*)calloc(sizeof(char), 1024);

    snprintf(id, 1024, "%s/0", ui->deviceProfileTaxiDBComboBox->itemData(ui->deviceProfileTaxiDBComboBox->currentIndex()).toString().toStdString().c_str());

    oyOptions_SetFromText(&options, "//" OY_TYPE_STD "/db/TAXI_id",
                          id,
                          OY_CREATE_NEW);

    ip = oyProfile_FromTaxiDB(options, NULL);

    oyOptions_Release(&options);

    oyOptions_SetFromText(&options,
			  "////device", "1",
			  OY_CREATE_NEW);

    int error = oyProfile_Install(ip, options);

    if(!ip) {
        // msgWidget->setMessageType(QMessageBox::Information);
	ui->msgWidget->setText(i18n("No valid profile obtained"));
        oyMessageFunc_p( oyMSG_ERROR, NULL, "%s", i18n("No valid profile obtained").toLocal8Bit().data());
    }

    if(error == oyERROR_DATA_AMBIGUITY) {
	// msgWidget->setMessageType(QMessageBox::Information);
	ui->msgWidget->setText(i18n("Profile already installed"));
        oyMessageFunc_p( oyMSG_ERROR, NULL, "%s", i18n("Profile already installed").toLocal8Bit().data());
        setProfile( QString::fromLocal8Bit(oyProfile_GetFileName( ip, 0 )) );
        updateProfileList( currentDevice, false );
    } else if(error == oyERROR_DATA_WRITE) {
	// msgWidget->setMessageType(QMessageBox::Error);
	ui->msgWidget->setText(i18n("User Path can not be written"));
        oyMessageFunc_p( oyMSG_ERROR, NULL, "%s", i18n("User Path can not be written").toLocal8Bit().data());
    } else if(error == oyCORRUPTED) {
	// msgWidget->setMessageType(QMessageBox::Error);
	ui->msgWidget->setText(i18n("Profile not useable"));
        oyMessageFunc_p( oyMSG_ERROR, NULL, "%s", i18n("Profile not useable").toLocal8Bit().data());
    } else if(error > 0) {
	QString text = i18n("Internal error") + " - " + QString::number(error);
        oyMessageFunc_p( oyMSG_ERROR, NULL, "%s", text.toLocal8Bit().data());
	// msgWidget->setMessageType(QMessageBox::Error);
	ui->msgWidget->setText(text);
    } else {
	// msgWidget->setMessageType(QMessageBox::Positive);
	ui->msgWidget->setText(i18n("Profile has been installed"));
        setProfile( QString::fromLocal8Bit(oyProfile_GetFileName( ip, 0 )) );
        updateProfileList( currentDevice, false );
    }

    oyOptions_Release(&options);
    oyProfile_Release(&ip);
}

// small helper to obtain a profile from a device
int SyDeviceGetProfile( oyConfig_s * device, oyProfile_s ** profile )
{
  oyOptions_s * options = 0;
  oyOptions_SetFromText( &options,
                   "//"OY_TYPE_STD"/config/icc_profile.x_color_region_target",
                         "yes", OY_CREATE_NEW );
  int error = oyDeviceGetProfile( device, options, profile );
  oyOptions_Release( &options );
  return error;
}

// obtain the Taxi DB query result
void SyDevicesModule::getTaxiSlot( char * for_device, oyConfigs_s * taxi_devices )
{
    int count = oyConfigs_Count(taxi_devices);

    int32_t rank = 0;
    oyConfig_s * taxi_device;
    oyConfig_s * device = getCurrentDevice();

    if(!oyConfig_FindString( device, "device_name", for_device) )
    {
      QString text = i18n("wrong device") + " ... " + QString::fromLocal8Bit(for_device);
      if(oy_debug)
        ui->msgWidget->setText(text);
      goto clean_getTaxiSlot;
    }

    for (int i = 0; i < count; i++) {
	taxi_device = oyConfigs_Get( taxi_devices, i );

	oyConfig_Compare(device, taxi_device, &rank);

	if (rank > 0) {
	    QString text = "[" + QString::number(rank) + "]";
	    text += " ";
	    text += oyConfig_FindString(taxi_device, "TAXI_profile_description", 0);
	    ui->deviceProfileTaxiDBComboBox->addItem(text, oyConfig_FindString(taxi_device, "TAXI_id", 0));
	}
        oyConfig_Release(&taxi_device);
    }

    // msgWidget->setMessageType(QMessageBox::Information);
    if (ui->deviceProfileTaxiDBComboBox->count() > 0) {
	ui->msgWidget->setText(i18n("You can select and install a profile"));
	ui->installProfileButton->setEnabled(true);
        ui->deviceProfileTaxiDBComboBox->setEnabled(true);
    } else {
	ui->msgWidget->setText(i18n("Not found any profile for the selected device in Taxi DB"));
	ui->installProfileButton->setEnabled(false);
    }

  clean_getTaxiSlot:
    oyConfigs_Release(&taxi_devices);
    oyConfig_Release(&device);
    if( for_device ) free( for_device );
}

// Set a new Profile and update UI.
void SyDevicesModule::setProfile( QString baseFileName )
{
    //emit changed(true);
    listModified = true;

    assignProfile( baseFileName );

    // Convert QString to proper C string.
    QByteArray raw_string;
    raw_string = (currentDevice->text(DEVICE_NAME)).toLocal8Bit();
    setCurrentDeviceName(raw_string.data());

    // Update column width of device list.
    for(int i = 0; i < ui->deviceList->columnCount(); i++)
        ui->deviceList->resizeColumnToContents(i);

    // Get the device that the user selected.
    oyConfig_s * device = 0;
    device = getCurrentDevice();

    oyConfig_Release(&device);
}

void SyDevicesModule::updateLocalProfileList(QTreeWidgetItem * selected_device,
                                 bool new_device)
{
    if(!selected_device)
    {
      ui->deviceProfileTaxiDBComboBox->clear();
      ui->deviceProfileTaxiDBComboBox->setEnabled(false);

      return;
    }

    // Don't count top parent items as a "selected device".
    if (selected_device->parent() == NULL)
    {
      ui->deviceProfileTaxiDBComboBox->setEnabled(false);

      return;
    }

    // The user modifies the list, but clicks away from the selected device item.
    listModified = false;

    // If we click on a device item, the current device is stored and options are available.
    ui->deviceProfileTaxiDBComboBox->setEnabled(true);

    currentDevice = selected_device;

    // Convert QString to proper C string.
    QByteArray raw_string;
    SyDevicesItem * device_item = dynamic_cast<SyDevicesItem*>(selected_device);
    raw_string = device_item->getText(DEVICE_NAME).toLocal8Bit();
    setCurrentDeviceName(raw_string.data());

    char * device_class = 0;
    if(selected_device && selected_device->parent())
    {
      QVariant v = selected_device->parent()->data( 0, Qt::UserRole );
      QString qs_device_class = v.toString();
      QByteArray raw_string;
      raw_string = qs_device_class.toLocal8Bit();
      device_class = strdup(raw_string.data());
    }

    // Change "Available Device Profiles" combobox to device-related profiles.
    if ( device_class )
      setCurrentDeviceClass(device_class);
}


// ************** Private Functions ********************

// Helper to obtain device string ID.
QString getDeviceName(oyConfig_s * device)
{ 
    const char * manufacturer = oyConfig_FindString( device,"manufacturer",0);
    const char * model = oyConfig_FindString( device, "model", 0);
    const char * serial = oyConfig_FindString( device, "serial", 0); 
    return QString::fromLocal8Bit(manufacturer)+" "+QString::fromLocal8Bit(model)+" "+QString::fromLocal8Bit(serial);  
}

int SyDevicesModule::installTaxiProfile(oyConfig_s * device)
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
int SyDevicesModule::checkProfileUpdates(oyConfig_s * device)
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
           else
           {
             QMessageBox msgBox;
             msgBox.setText("Could not install " 
                     + getDeviceName(device) + ".");
             msgBox.setDefaultButton(QMessageBox::Yes);
             ret = msgBox.exec();
           }
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

// General function to detect and retrieve devices via the Oyranos CMS backend.
int SyDevicesModule::detectDevices(const char * device_type)
{ 
    int error = 0;
    oyConfigs_s * device_list = 0;
    oyOptions_s * options = oyOptions_New(0);
    oyConfDomain_s * d = oyConfDomain_FromReg( device_type, 0 );
    const char * reg_app = strrchr(device_type,'/')+1;
    const char * device_class = oyConfDomain_GetText( d, "device_class",
                                                      oyNAME_NICK );
    const char * device_class_ui = oyConfDomain_GetText( d, "device_class",
                                                      oyNAME_NAME );

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
        device_class_item->setText( ITEM_MAIN, QString::fromLocal8Bit(device_class_ui) );
        QVariant v( device_class );
        device_class_item->setData( 0, Qt::UserRole, v );
        ui->deviceList->insertTopLevelItem( ITEM_MAIN, device_class_item );

        QIcon device_icon;
        QSize icon_size(64, 64);
        QString iconPath = QString(":/resources/") + QString::fromLocal8Bit(device_class) + ".png";
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
                deviceItemString.append(QString::fromLocal8Bit(device_manufacturer));
                deviceItemString.append(" ");
            }

            deviceItemString.append(QString::fromLocal8Bit(device_model));
            if(device_serial)
            {
              deviceItemString.append(" ");
              deviceItemString.append(QString::fromLocal8Bit(device_serial));
            }
            
            // TESTING Check device for Taxi updates.
            //if (strcmp(device_class, "monitor") == 0)
            //  checkProfileUpdates(device);

            error = syDeviceGetProfile(device, icc_profile_flags, &profile);
            profile_filename = oyProfile_GetFileName(profile, 0);
 
            SyDevicesItem * deviceItem = new SyDevicesItem(0);

            if (profile_filename == NULL)
            {
                deviceProfileDescription = QString::fromLocal8Bit( oyProfile_GetText( profile, oyNAME_DESCRIPTION ) );
                if(!deviceProfileDescription.count())
                  deviceProfileDescription = "(No Profile Installed!)";
                profile_filename = "------";
            }
            else
                deviceProfileDescription = convertFilenameToDescription(QString::fromLocal8Bit(profile_filename));

            deviceItem->setIcon( ITEM_MAIN, device_icon );
            
            deviceItem->addText(DEVICE_DESCRIPTION, deviceItemString);
            deviceItem->addText(DEVICE_NAME, QString::fromLocal8Bit(device_designation));
            deviceItem->addText(PROFILE_DESCRIPTION, deviceProfileDescription);   
            deviceItem->addText(PROFILE_FILENAME, QString::fromLocal8Bit(profile_filename));
            deviceItem->setDevice(device);

            deviceItem->refreshText();
    
            // add association combo box in tree widget.
            SyDeviceItem * profileAssociationCB = new SyDeviceItem();
            connect( profileAssociationCB, SIGNAL(currentIndexChanged( int )),
                     this, SLOT( changeDeviceItem( int )) );
            QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
            layout->addWidget (profileAssociationCB);
            profileAssociationCB->setParent(deviceItem);

            QWidget * w = new QWidget();
            w->setLayout(layout);
 
            device_class_item->addChild( deviceItem );
            ui->deviceList->setItemWidget( deviceItem, ITEM_COMBOBOX, w);

            updateProfileList( deviceItem, false );

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
void SyDevicesModule::populateDeviceListing()
{
    // TODO Work out a solution to use raw/camera stuff.

    uint32_t count = 0, i = 1,
           * rank_list = 0;
    char ** texts = 0;

    // get all configuration filters
    oyConfigDomainList( "//"OY_TYPE_STD"/config.device.icc_profile",
                        &texts, &count, &rank_list ,0 );

    for (i = 0; i < count; i++)
    {
      detectDevices( texts[i] );
    }

}



// Populate "Assign Profile" combobox.  Depending on the device selected, the profile list will vary.
void SyDevicesModule::populateDeviceComboBox( QComboBox & itemComboBox, unsigned int sig, bool new_device )
{
    int size, i, current = -1, current_tmp, pos = 0;
    oyProfile_s * profile = 0, * temp_profile = 0;
    oyProfiles_s * patterns = oyProfiles_New(0),
                 * iccs = 0;
    oyConfig_s * device = getCurrentDevice();
    const char * profile_file_name = 0;
    icProfileClassSignature deviceSignature = (icProfileClassSignature) sig;

    profile = oyProfile_FromSignature( deviceSignature, oySIGNATURE_CLASS, 0 );
    oyProfiles_MoveIn( patterns, &profile, -1 );

    iccs = oyProfiles_Create( patterns, icc_profile_flags, 0 );
    oyProfiles_Release( &patterns );
 
    QString getProfileDescription;

    size = oyProfiles_Count(iccs);
    int32_t * rank_list = (int32_t*) malloc( oyProfiles_Count(iccs) *
                                             sizeof(int32_t) );
    oyProfiles_DeviceRank( iccs, device, rank_list );

    size = oyProfiles_Count(iccs);

    itemComboBox.clear();

    syDeviceGetProfile( device, icc_profile_flags, &profile ); /* reget profile */
    profile_file_name = oyProfile_GetFileName( profile, 0 );

    Qt::CheckState show_only_device_related = 
                                          ui->relatedDeviceCheckBox->checkState();
    int empty_added = -1;

    for( i = 0; i < size; ++i)
    {
      const char * temp_profile_file_name;
         temp_profile = oyProfiles_Get( iccs, i );
         // show rank number
         getProfileDescription = "[" + QString::number(rank_list[i]) + "] ";

         getProfileDescription += QString::fromLocal8Bit( oyProfile_GetText( temp_profile, oyNAME_DESCRIPTION ) );
         temp_profile_file_name = oyProfile_GetFileName( temp_profile, 0 );
 
         current_tmp = -1;

         if(profile_file_name && temp_profile_file_name &&
            strcmp( profile_file_name, temp_profile_file_name ) == 0)
           current_tmp = pos;

         if(current == -1 && current_tmp != -1)
           current = current_tmp;

         if(empty_added == -1 &&
            rank_list[i] < 1)
         {
           itemComboBox.addItem(i18n("automatic"));
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
           getProfileDescription.append(QString::fromLocal8Bit(temp_profile_file_name));
           getProfileDescription.append(")");
           itemComboBox.addItem(getProfileDescription, QString::fromLocal8Bit(temp_profile_file_name));
           ++pos;
         }
      oyProfile_Release( &temp_profile );
    }
    
    if(empty_added == -1)
    {
      itemComboBox.addItem(i18n("automatic"));
      ++pos;
      if(current == -1 && current_tmp != -1)
        current = pos;
    }
    
    if(current == -1 && profile_file_name)
    {
      getProfileDescription = QString::fromLocal8Bit( oyProfile_GetText( profile, oyNAME_DESCRIPTION ) );
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

oyConfig_s * SyDevicesModule::getCurrentDevice( void )
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

class SySleep : public QThread
{
  public:
     static void sleep(double seconds)
     { QThread::msleep((long unsigned int)(seconds*1000)); }
};

void SyDevicesModule::assignProfile( QString profile_name )
{        
     oyProfile_s * profile = 0;
     QString description;

     // If current device pointer points to a MONITOR item, save default profile to Oyranos.

     {
         oyConfig_s * device = getCurrentDevice();
         const char * profilename = profile_name.toLocal8Bit();
         char * pn = strdup(profilename);

         /* store a existing profile in DB */
         if(strlen(pn) && QString::localeAwareCompare( QString(pn), i18n("automatic")))
           oyDeviceSetProfile ( device, pn );
         oyDeviceUnset( device ); /* unset the device */
         /* completly unset the actual profile from DB */
         if(!strlen(pn) || !QString::localeAwareCompare(QString(pn), i18n("automatic")))
         {
           oyConfig_EraseFromDB( device );
           oyConfig_Release( &device );
           /* reopen the device to forget about the "profile_name" key */
           device = getCurrentDevice();
         }
         oyDeviceSetup( device ); /* reinitialise */
         /* compiz needs some time to exchange the profiles,
            immediately we would get the old colour server profile */
         SySleep::sleep(0.3);
         syDeviceGetProfile( device, icc_profile_flags, &profile ); /* reget profile */

         /* clear */
         oyConfig_Release( &device );
         if(pn) free(pn); pn = 0;
     }

     // Convert profile into description name...
     description = QString::fromLocal8Bit( oyProfile_GetText( profile, oyNAME_DESCRIPTION) );

     if(!description.count())
       description = "(No Profile Installed!)";

     if(!profile_name.count())
       profile_name = "------";
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
    int n, i = 0;
    int32_t * ranks;
    oyConfig_s * taxi_dev;
    oyConfigs_s * devices = 0;
    oyOptions_s * options = NULL;
    
    oyOptions_SetFromText(&options,
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
        oyConfig_Compare(device, taxi_dev, &ranks[2*i+1]);

        oyConfig_Release(&taxi_dev);
      }
      
      // TODO Fix ranking so that it uses C++ sorting algorithm instead.
      qsort(ranks, n, sizeof(int32_t)*2, compareRanks);      
      
      // Obtain the best-ranked device profile (0).
      taxi_dev = oyConfigs_Get(devices, ranks[0]);

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
QString SyDevicesModule::getTaxiString(oyConfig_s * device, const char * oy_taxi_string)
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
QString SyDevicesModule::checkRecentTaxiProfile(oyConfig_s * device)
{    
    QString profileDescription = getTaxiString(device, "TAXI_profile_description");     
    return profileDescription;
}

// Download a profile from Taxi server and return its filename.
QString SyDevicesModule::downloadTaxiProfile(oyConfig_s * device)
{  
    QString fileName = "";
    int error;
    oyProfile_s * ip = 0;    
    oyOptions_s * options = NULL;
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
      fileName = QString(TAXI_DOWNLOAD_PATH) + OY_SLASH + QString::fromLocal8Bit( taxi_profile_name )
                 + ".icc";
      
      data = (char*)oyProfile_GetMem(ip, &size, 0, malloc);
            
      if(data)
      {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
      
        QDataStream out(&file);      
        out.writeRawData(data, size);
        qWarning("installed -> %s", fileName.toLocal8Bit().data());

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
QString SyDevicesModule::convertFilenameToDescription(QString profileFilename)
{    
    QString profileDescriptionName;
    oyProfile_s * profile;
    
    profile = oyProfile_FromFile( profileFilename.toLocal8Bit(), 0, 0);
    profileDescriptionName = QString::fromLocal8Bit( oyProfile_GetText( profile, oyNAME_DESCRIPTION ) );
    oyProfile_Release( &profile );

    return profileDescriptionName;
}


SyDevicesModule::~SyDevicesModule()
{
  delete ui; 
}

void TaxiLoad::run()
{
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
