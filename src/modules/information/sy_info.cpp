 
#include <oyranos.h>
#include <oyranos_devices.h>
#include <oyranos_icc.h>
#include <oyFilterNode_s.h>
#include <oyProfiles_s.h>

#include <QProcess>
#include <QTimer>
#include <QtDBus/QtDBus>
 
#include "sy_info.h"
#include "sy_info_config.h"

// Qt Designer code translation.
#include "ui_sy_info.h"          

const char * sy_info_module_name = "Information";

SyInfoModule::SyInfoModule(QWidget * parent)
    : SyModule(parent)
{       
    current_profile = NULL;

    const char    * name = NULL,
                  * description = NULL;

    oyWidgetTitleGet( oyWIDGET_GROUP_INFORMATION, NULL, &name,
                      NULL, NULL );
    oyWidgetDescriptionGet( oyWIDGET_GROUP_INFORMATION, &description, 0 );
    setModuleName(QString::fromLocal8Bit(name));
    setDescription(QString::fromLocal8Bit(description));

    infoDialog = new SyInfoDialog(this);

    // select profiles matching actual capabilities
    icc_profile_flags = oyICCProfileSelectionFlagsFromOptions( OY_CMM_STD, "//" OY_TYPE_STD "/icc_color", NULL, 0 );

    ui = new Ui::syInfoWidget();
    ui->setupUi(this);                  // Load Gui.

    SyInfoConfig * infoConfig = new SyInfoConfig(0, sy_info_module_name);
    setConfigWidget( infoConfig );

    setEditable(false);

    ui->installedProfilesTree->setColumnWidth(0, 350);
    ui->installedProfilesTree->setColumnWidth(1, 150);

    ui->installedProfilesTree->expandAll();

    examineIcon.addFile(QString::fromLocal8Bit(":/resources/examine.png"),
                                  QSize(10, 10), QIcon::Normal, QIcon::On);
 
    examineIcon.addFile(QString::fromLocal8Bit(":/resources/examine_select.png"),
                                  QSize(10, 10), QIcon::Active, QIcon::On);

    populateInstalledProfileList(true);

    ui->installedProfilesTree->expandAll();

    connect( ui->installedProfilesTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
             this, SLOT(profileExamineButtonClicked(QTreeWidgetItem *, int)));

    if( QDBusConnection::sessionBus().connect( QString(), "/org/libelektra/configuration", "org.libelektra", QString(),
                                               this, SLOT( configChanged( QString ) )) )
        fprintf(stderr, "=================== connect info to libelektra\n" );
    acceptDBusUpdate = true;
}


//  ******** SIGNAL/SLOT Functions *****************

void SyInfoModule::configChanged( QString msg )
{
  if(acceptDBusUpdate == false) return;
  acceptDBusUpdate = false;

  QTimer::singleShot(250, this, SLOT(populateInstalledProfileList()));
};

void SyInfoModule::profileExamineButtonClicked(QTreeWidgetItem * currentProfileItem, int column)
{
    if (column != ITEM_ICON)
       return;

    QVariant v = currentProfileItem->data( 0, Qt::UserRole );
    oyProfile_s * p = (oyProfile_s *) v.toULongLong();

    if(p && p->type_ == oyOBJECT_PROFILE_S)
    {
      infoDialog->showDialog();

      populateDeviceProfileDescriptions(p, true);
      return;
    }

    populateDeviceProfileDescriptions(NULL, false);
}


// ************** Private Functions ********************
void SyInfoModule::populateInstalledProfileList()
{
  // clear the Oyranos settings cache
  oyGetPersistentStrings( NULL );

  populateInstalledProfileList(true);
  acceptDBusUpdate = true;
}

// Populate the tree with detected profile items.
void SyInfoModule::populateInstalledProfileList(bool init)
{
    if(init == false &&
       ui->installedProfilesTree->isVisible() == false)
      return;

    ui->installedProfilesTree->clear();

    QTreeWidgetItem * devicesTree = new QTreeWidgetItem;
    ui->installedProfilesTree->addTopLevelItem( devicesTree );

    const char * g_name = NULL;
    oyWidgetTitleGet( oyWIDGET_GROUP_DEVICES, NULL, &g_name,
                      NULL, NULL );
    devicesTree->setText( ITEM_NAME, QString::fromLocal8Bit(g_name));
    populateDeviceProfiles( devicesTree );

    // Save tree list parents to QTreeWidgetItem pointers.
    QTreeWidgetItem * editingCsTree = new QTreeWidgetItem;
    ui->installedProfilesTree->addTopLevelItem( editingCsTree );
    oyWidgetTitleGet( oyWIDGET_GROUP_DEFAULT_PROFILES_EDIT, NULL, &g_name,
                      NULL, NULL );
    editingCsTree->setText( ITEM_NAME, QString::fromLocal8Bit(g_name));
    QTreeWidgetItem * assumedCsTree = new QTreeWidgetItem;
    ui->installedProfilesTree->addTopLevelItem( assumedCsTree );
    oyWidgetTitleGet( oyWIDGET_GROUP_DEFAULT_PROFILES_ASSUMED, NULL, &g_name,
                      NULL, NULL );
    assumedCsTree->setText( ITEM_NAME, QString::fromLocal8Bit(g_name));
    QTreeWidgetItem * effectCsTree = new QTreeWidgetItem;
    ui->installedProfilesTree->addTopLevelItem( effectCsTree );
    oyWidgetTitleGet( oyWIDGET_GROUP_BEHAVIOUR_EFFECT, NULL, &g_name,
                      NULL, NULL );
    effectCsTree->setText( ITEM_NAME, QString::fromLocal8Bit(g_name));

    // For convenience, we expand colorspace trees.
    ui->installedProfilesTree->expandItem(editingCsTree);
    ui->installedProfilesTree->expandItem(assumedCsTree);
    ui->installedProfilesTree->expandItem(effectCsTree);

    // Populate colorspace items.
    oyWidgetTitleGet( (oyWIDGET_e)oyEDITING_RGB, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
        addProfileTreeItem( oyEDITING_RGB, QString::fromLocal8Bit(g_name), editingCsTree);
    oyWidgetTitleGet( (oyWIDGET_e)oyEDITING_CMYK, NULL, &g_name, NULL,NULL );
    if (strlen(g_name) > 0)
       addProfileTreeItem( oyEDITING_CMYK, QString::fromLocal8Bit(g_name), editingCsTree);
    oyWidgetTitleGet( (oyWIDGET_e)oyEDITING_XYZ, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
        addProfileTreeItem( oyEDITING_XYZ, QString::fromLocal8Bit(g_name), editingCsTree );
    oyWidgetTitleGet( (oyWIDGET_e)oyEDITING_LAB, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
        addProfileTreeItem( oyEDITING_LAB, QString::fromLocal8Bit(g_name), editingCsTree );
    oyWidgetTitleGet( (oyWIDGET_e)oyEDITING_GRAY, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
        addProfileTreeItem( oyEDITING_GRAY, QString::fromLocal8Bit(g_name), editingCsTree );
    oyWidgetTitleGet( (oyWIDGET_e)oyASSUMED_RGB, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
       addProfileTreeItem( oyASSUMED_RGB, QString::fromLocal8Bit(g_name), assumedCsTree );
    oyWidgetTitleGet( (oyWIDGET_e)oyASSUMED_CMYK, NULL, &g_name, NULL,NULL );
    if (strlen(g_name) > 0)
        addProfileTreeItem( oyASSUMED_CMYK, QString::fromLocal8Bit(g_name), assumedCsTree );
    oyWidgetTitleGet( (oyWIDGET_e)oyASSUMED_XYZ, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
       addProfileTreeItem( oyASSUMED_XYZ, QString::fromLocal8Bit(g_name), assumedCsTree );
    oyWidgetTitleGet( (oyWIDGET_e)oyASSUMED_LAB, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
       addProfileTreeItem( oyASSUMED_LAB, QString::fromLocal8Bit(g_name), assumedCsTree );
    oyWidgetTitleGet( (oyWIDGET_e)oyASSUMED_GRAY, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
       addProfileTreeItem( oyASSUMED_GRAY, QString::fromLocal8Bit(g_name), assumedCsTree );
    oyWidgetTitleGet( (oyWIDGET_e)oyWIDGET_PROFILE_PROOF, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
       addProfileTreeItem( oyPROFILE_PROOF, QString::fromLocal8Bit(g_name), effectCsTree );
    oyWidgetTitleGet( (oyWIDGET_e)oyWIDGET_PROFILE_EFFECT, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
       addProfileTreeItem( oyPROFILE_EFFECT, QString::fromLocal8Bit(g_name), effectCsTree );

    ui->installedProfilesTree->expandAll();
}


// Function to add profile items into the tree listing.
void SyInfoModule::addProfileTreeItem( unsigned int type, QString description, 
                                        QTreeWidgetItem * parent_item )
{
    oyPROFILE_e profile_type = (oyPROFILE_e) type;
    oyProfile_s * profile = oyProfile_FromStd( profile_type, icc_profile_flags, 0);
    const char * text = oyProfile_GetText( profile, oyNAME_DESCRIPTION );

    // Add new item.
    QTreeWidgetItem * new_child = new QTreeWidgetItem();
    
    new_child->setIcon( ITEM_ICON, examineIcon );
    new_child->setText( ITEM_DESCRIPTION, text );
    new_child->setText( ITEM_NAME, description );

    // attach the profile to the widget
    oyProfile_Copy(profile,0);
    QVariant v((qulonglong) profile);
    new_child->setData( ITEM_NAME, Qt::UserRole, v );
    
    ui->installedProfilesTree->setUniformRowHeights(false);
    ui->installedProfilesTree->setAllColumnsShowFocus(false);;
    
    parent_item->addChild(new_child);    
    
    oyProfile_Release( &profile );
} 


void SyInfoModule::populateDeviceProfiles( QTreeWidgetItem * deviceListTree )
{
    int n = 0;

    uint32_t count = 0, j,
           * rank_list = 0;
    int32_t i;
    char ** texts = 0;
    oyConfDomain_s * d = 0;

    // get all configuration filters
    oyConfigDomainList( "//" OY_TYPE_STD "/device/config.icc_profile",
                        &texts, &count, &rank_list ,0 );

    for(i = count - 1; i >= 0; --i)
    {
      QIcon device_icon("");
      oyConfigs_s * devices = 0;
      const char * reg_app = strrchr(texts[i],'/')+1;
      oyDevicesGet( OY_TYPE_STD, reg_app, 0, &devices );
      n = oyConfigs_Count( devices );
      d = oyConfDomain_FromReg( texts[i], 0 );

      // Setup icons for devices parent item.
      if(strstr(reg_app,"monitor"))
        device_icon = QIcon(":/resources/monitor.png");
      else if(strstr(reg_app,"printer"))
        device_icon = QIcon(":/resources/printer.png");
      else if(strstr(reg_app,"scanner"))
        device_icon = QIcon(":/resources/scanner.png");

      QTreeWidgetItem * device_list_sub_tree = deviceListTree;
#     if 0
      // add subtree per device class
      // pick the modules device class nick name
      const char * device_class = oyConfDomain_GetText( d, "device_class", oyNAME_NAME );

      device_list_sub_tree->setText( ITEM_NAME, device_class );
      deviceListTree->addChild( device_list_sub_tree );
#     endif

      for(j = 0; j < (uint32_t)n; ++j)
      {
        oyConfig_s * device = oyConfigs_Get( devices, j );
        char * device_info = 0;
        oyOptions_s * options = 0;
        oyOptions_SetFromText( &options, "//" OY_TYPE_STD "/config/icc_profile.x_color_region_target",
                                         "yes", OY_CREATE_NEW );
        oyOptions_SetFromInt( &options, "///icc_profile_flags", icc_profile_flags, 0, OY_CREATE_NEW );

        // get expensive informations to see the "model" option
        oyDeviceGetInfo( device, oyNAME_DESCRIPTION, options,
                                 &device_info, malloc );
        oyDeviceGetInfo( device, oyNAME_NAME, options,
                                 &device_info, malloc );

        QTreeWidgetItem * device_child = new QTreeWidgetItem;
        QString device_item_string;
        const char * device_manufacturer = 0;
        const char * device_model = 0;
        const char * device_serial = 0;   

        device_manufacturer = oyConfig_FindString( device, "manufacturer", 0);
        device_model = oyConfig_FindString( device, "model", 0);
        device_serial = oyConfig_FindString( device, "serial", 0);  

        device_item_string.append(device_manufacturer);
        device_item_string.append(" ");
        device_item_string.append(device_model);
        device_item_string.append(" ");
        device_item_string.append(device_serial);

        char * model = 0;
        if(device_manufacturer || device_model || device_serial)
          model = strdup(device_item_string.toUtf8());
        else
        {
          const char * m = oyConfig_FindString( device, "device_name", 0);
          if(m)
            model = strdup(m);
          else
            oyDeviceGetInfo(device, oyNAME_NICK, 0, &model, malloc);
        }

#if 0
        if(device_info)
          device_child->setText(0, QString("\"") + model + "\" "
                                   + QString(device_info));
        else
#endif
        if(model)
          device_child->setText( ITEM_NAME, QString(model) );
        if(model) free(model); model = 0;
        device_child->setIcon(ITEM_NAME, device_icon);
        device_list_sub_tree->addChild(device_child);   

        oyProfile_s * p = 0;
        oyDeviceAskProfile2( device, options, &p );

        if(p)
        {
          const char* profile_name = oyProfile_GetText( p, oyNAME_DESCRIPTION );
#         if 0
          const char* file_name = oyProfile_GetFileName( p, -1 );
          if(file_name)
            device_child->setText( ITEM_NAME, device_child->text(ITEM_NAME) + "\n" + QString(file_name) );
          else
            device_child->setText( ITEM_NAME, device_child->text(ITEM_NAME) + "\n" + "in memory" );
#         endif
          device_child->setText( ITEM_DESCRIPTION, profile_name );

          // attach the profile to the widget
          oyProfile_Copy(p,0);
          QVariant v((qulonglong) p);
          device_child->setData( 0, Qt::UserRole, v );

          device_child->setIcon(ITEM_ICON, examineIcon);
        }

        oyProfile_Release( &p );
        oyConfig_Release( &device );
        oyOptions_Release( &options );
      }
      oyConfigs_Release( &devices );
      oyConfDomain_Release( &d );
    }
}

// Refresh profile information list.
void SyInfoModule::populateDeviceProfileDescriptions(oyProfile_s * profile, bool valid)
{
    if (valid)
    {
        // Output Oyranos-specified profile descriptions.
        setTagDescriptions(profile, icSigCopyrightTag, COPYRIGHT_TAG);        
	setTagDescriptions(profile, icSigDeviceMfgDescTag, MANUFACTURER_TAG);        
	setTagDescriptions(profile, icSigDeviceModelDescTag, MODEL_TAG);        
	setTagDescriptions(profile, icSigProfileDescriptionTag, DESCRIPTION_TAG);

        setDateTag(profile);
	
        setColorSpaceTag(profile);
	
        setIccsTag(profile);
	
        setPcsTag(profile);
	
        setDeviceClassTag(profile);

        QString profilePathName = oyProfile_GetFileName( profile, -1 );
        infoDialog->setDialogText( PROFILE_PATH_TAG, profilePathName );

	m_tempFile.open();

	QProcess * process = new QProcess();
	connect(process, SIGNAL(finished(int)), SLOT(loadProfileGraph()));

	QString program = QString("oyranos-profile-graph -o \"%1\" -w 200 \"%2\"").arg(m_tempFile.fileName()).arg(profilePathName);
	process->start(program);

        oyProfile_Release( &current_profile );
        current_profile = oyProfile_Copy( profile, 0 );
    }
    else
    {
        // Set default descriptions.
        infoDialog->setDialogText( COLORSPACE_TAG, "" );
        infoDialog->setDialogText( COPYRIGHT_TAG, tr("(Copyright not available)") );
        infoDialog->setDialogText( DATE_CREATED_TAG, "" );
        infoDialog->setDialogText( DESCRIPTION_TAG, tr("No Profile Selected") );
        infoDialog->setDialogText( DEVICE_TAG, "" );
        infoDialog->setDialogText( FILENAME_TAG, "" );
        infoDialog->setDialogText( ICC_VERSION_TAG, "" );
        infoDialog->setDialogText( MANUFACTURER_TAG, "" );
        infoDialog->setDialogText( MODEL_TAG, "" );
        infoDialog->setDialogText( PCS_TAG, "" );
        infoDialog->setDialogText( PROFILE_PATH_TAG, "" );
        infoDialog->loadProfileGraph(QString(""));
    }
  
}

void SyInfoModule::loadProfileGraph()
{
        infoDialog->loadProfileGraph(m_tempFile.fileName());

        m_tempFile.close();
}

// Function to write signature head, based on profile, tag type, and QT Label.
void SyInfoModule::setTagDescriptions(oyProfile_s * profile_name, unsigned int type, DialogString tag )
{
     int text_n;
     bool error;
     icTagSignature tagType = (icTagSignature) type;
     
     //oyObject_s  object;
     char** tagText = 0;
     oyProfileTag_s * tagID;

     tagID = oyProfile_GetTagById( profile_name, tagType  );
    
     error = !tagID;
     
     if(!error)
         tagText = oyProfileTag_GetText( tagID, &text_n, 0,0, 0, 0 );
     
     if(text_n && tagText && tagText[0])                  
         infoDialog->setDialogText(tag, *tagText);
     else
         infoDialog->setDialogText(tag, "");
}


void SyInfoModule::setPcsTag(oyProfile_s * profile)
{
     QString tagString;
     tagString = oyICCColorSpaceGetName( (icColorSpaceSignature)
                         oyProfile_GetSignature(profile, oySIGNATURE_PCS) );
     infoDialog->setDialogText( PCS_TAG, QString("CIE") + tagString);
     
}


void SyInfoModule::setColorSpaceTag(oyProfile_s * profile)
{
     QString tagString;
     tagString = oyICCColorSpaceGetName( (icColorSpaceSignature)
                         oyProfile_GetSignature(profile, oySIGNATURE_COLOR_SPACE) );
     infoDialog->setDialogText( COLORSPACE_TAG, tagString);     
}


void SyInfoModule::setIccsTag(oyProfile_s * profile)
{
    QString field1, field2, field3;

    icSignature vs = oyValueUInt32( oyProfile_GetSignature(profile, oySIGNATURE_VERSION) );
    char * v = (char*)&vs;
    
    field1 = (((int)v[0]));
    field2 = ((int)v[1]/16);
    field3 = ((int)v[1]%16);
    
    infoDialog->setDialogText( ICC_VERSION_TAG,  field1.setNum(((int)v[0])) + "." 
                                               + field2.setNum(((int)v[1]/16)) + "." 
                                               + field3.setNum(((int)v[1]%16)));    
}


void SyInfoModule::setDeviceClassTag(oyProfile_s * profile)
{
     QString string;
     string = oyICCDeviceClassDescription( (icProfileClassSignature)
                         oyProfile_GetSignature(profile, oySIGNATURE_CLASS) );
     infoDialog->setDialogText( DEVICE_TAG, string );     
}


void SyInfoModule::setDateTag(oyProfile_s * profile)
{
     uint year, month, day; 
     //uint hours, minutes, seconds;
     QString yearString, monthString, dayString;

     year = oyProfile_GetSignature(profile, oySIGNATURE_DATETIME_YEAR);
     month = oyProfile_GetSignature(profile, oySIGNATURE_DATETIME_MONTH);
     day = oyProfile_GetSignature(profile, oySIGNATURE_DATETIME_DAY);
     
     // NOTE (If we need hours/minutes/seconds, uncomment)
      //hours = oyProfile_GetSignature(profile, oySIGNATURE_DATETIME_HOURS);
      //minutes = oyProfile_GetSignature(profile, oySIGNATURE_DATETIME_MINUTES);
      //seconds = oyProfile_GetSignature(profile, oySIGNATURE_DATETIME_SECONDS);

     infoDialog->setDialogText(DATE_CREATED_TAG, 
                                 monthString.setNum(year) + "-" 
                                 + dayString.setNum(month) + "-" 
                                 + yearString.setNum(day));
}


SyInfoModule::~SyInfoModule()
{
    if( QDBusConnection::sessionBus().disconnect( QString(), "/org/libelektra/configuration", "org.libelektra", QString(),
                                              this, SLOT( configChanged( QString ) )) )
        fprintf(stderr, "=================== disconnect info from libelektra\n" );
    delete infoDialog;
}
