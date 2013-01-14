 
#include <oyranos.h>
#include <oyranos_devices.h>
#include <oyranos_icc.h>
#include <oyProfiles_s.h>

#include <QProcess>
 
#include "sy_info.h"
#include "sy_info_config.h"

const char * sy_info_module_name = "Information";

SyInfo::SyInfo(QWidget * parent)
    : SyModule(parent)
{       
    current_profile = NULL;
    
    infoDialog = new SyInfoDialog(this);
  
    setModuleName(sy_info_module_name);
    setDescription("Analyze profile information on your system.");
    
    setupUi(this);                  // Load Gui.
    
    SyInfoConfig * infoConfig = new SyInfoConfig(0, sy_info_module_name);
    setConfigWidget( infoConfig );
    
    setEditable(false);
        
    installedProfilesTree->setColumnWidth(0, 350);
    installedProfilesTree->setColumnWidth(1, 150);
    
    // Save tree list parents to QTreeWidgetItem pointers.
    devicesParentTree = installedProfilesTree->topLevelItem(0);
    const char * g_name = NULL;
    QString name;
    editingCsTree = installedProfilesTree->topLevelItem(1);
    oyWidgetTitleGet( oyWIDGET_GROUP_DEFAULT_PROFILES_EDIT, NULL, &g_name,
                      NULL, NULL );
    editingCsTree->setText( ITEM_NAME, name.fromLatin1(g_name));
    assumedCsTree = installedProfilesTree->topLevelItem(2);
    oyWidgetTitleGet( oyWIDGET_GROUP_DEFAULT_PROFILES_ASSUMED, NULL, &g_name,
                      NULL, NULL );
    assumedCsTree->setText( ITEM_NAME, name.fromLatin1(g_name));

    // For convenience, we expand colorspace trees.
    installedProfilesTree->expandItem(editingCsTree);
    installedProfilesTree->expandItem(assumedCsTree);

    installedProfilesTree->expandAll();
        
    examineIcon.addFile(QString::fromUtf8(":/resources/examine.png"), 
                                  QSize(10, 10), QIcon::Normal, QIcon::On);
 
    examineIcon.addFile(QString::fromUtf8(":/resources/examine_select.png"), 
                                  QSize(10, 10), QIcon::Active, QIcon::On);
    
    // Display oyEDITING_XYZ info for now. 
    populateInstalledProfileList();

    connect( installedProfilesTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), 
             this, SLOT(profileExamineButtonClicked(QTreeWidgetItem *, int)));

}


//  ******** SIGNAL/SLOT Functions *****************

void SyInfo::profileExamineButtonClicked(QTreeWidgetItem * currentProfileItem, int column)
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

// Populate the tree with detected profile items.
void SyInfo::populateInstalledProfileList()
{
    devicesParentTree = installedProfilesTree->topLevelItem(0);
    editingCsTree = installedProfilesTree->topLevelItem(1);
    assumedCsTree = installedProfilesTree->topLevelItem(2);

    populateDeviceProfiles( devicesParentTree );
    installedProfilesTree->expandAll();

    const char * g_name = NULL;
    QString name;
            
    // Populate colorspace items.
    oyWidgetTitleGet( (oyWIDGET_e)oyEDITING_RGB, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
        addProfileTreeItem( oyEDITING_RGB, name.fromLatin1(g_name), editingCsTree);
    oyWidgetTitleGet( (oyWIDGET_e)oyEDITING_CMYK, NULL, &g_name, NULL,NULL );
    if (strlen(g_name) > 0)
       addProfileTreeItem( oyEDITING_CMYK, name.fromLatin1(g_name), editingCsTree);
    oyWidgetTitleGet( (oyWIDGET_e)oyEDITING_XYZ, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
        addProfileTreeItem( oyEDITING_XYZ, name.fromLatin1(g_name), editingCsTree );
    oyWidgetTitleGet( (oyWIDGET_e)oyEDITING_LAB, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
        addProfileTreeItem( oyEDITING_LAB, name.fromLatin1(g_name), editingCsTree );
    oyWidgetTitleGet( (oyWIDGET_e)oyEDITING_GRAY, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
        addProfileTreeItem( oyEDITING_GRAY, name.fromLatin1(g_name), editingCsTree );
    oyWidgetTitleGet( (oyWIDGET_e)oyASSUMED_RGB, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
       addProfileTreeItem( oyASSUMED_RGB, name.fromLatin1(g_name), assumedCsTree );
    oyWidgetTitleGet( (oyWIDGET_e)oyASSUMED_CMYK, NULL, &g_name, NULL,NULL );
    if (strlen(g_name) > 0)
        addProfileTreeItem( oyASSUMED_CMYK, name.fromLatin1(g_name), assumedCsTree );
    oyWidgetTitleGet( (oyWIDGET_e)oyASSUMED_XYZ, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
       addProfileTreeItem( oyASSUMED_XYZ, name.fromLatin1(g_name), assumedCsTree );
    oyWidgetTitleGet( (oyWIDGET_e)oyASSUMED_LAB, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
       addProfileTreeItem( oyASSUMED_LAB, name.fromLatin1(g_name), assumedCsTree );
    oyWidgetTitleGet( (oyWIDGET_e)oyASSUMED_GRAY, NULL, &g_name, NULL, NULL );
    if (strlen(g_name) > 0)
       addProfileTreeItem( oyASSUMED_GRAY, name.fromLatin1(g_name), assumedCsTree );
}


// Function to add profile items into the tree listing.
void SyInfo::addProfileTreeItem( oyPROFILE_e profile_type, QString description, 
                                        QTreeWidgetItem * parent_item )
{
    oyProfile_s * profile = oyProfile_FromStd( profile_type, 0);
    const char * text = oyProfile_GetText( profile, oyNAME_DESCRIPTION );
      
    // Add new item.
    QTreeWidgetItem * new_child = new QTreeWidgetItem();
    
    new_child->setIcon( ITEM_ICON, examineIcon );
    new_child->setText( ITEM_DESCRIPTION, text );
    new_child->setText( ITEM_NAME, description );

    // attach the profile to the widget
    QVariant v((qulonglong) oyProfile_Copy(profile,0));
    new_child->setData( ITEM_NAME, Qt::UserRole, v );
    
    installedProfilesTree->setUniformRowHeights(false);
    installedProfilesTree->setAllColumnsShowFocus(false);;
    
    parent_item->addChild(new_child);    
    
    oyProfile_Release( &profile );
} 


void SyInfo::populateDeviceProfiles( QTreeWidgetItem * deviceListTree )
{
    int n = 0;
    QIcon device_icon("");

    uint32_t count = 0, i,j,
           * rank_list = 0;
    char ** texts = 0;
    const char * device_class = 0;
    oyConfDomain_s * d = 0;
    int error = 0;

    // get all configuration filters
    oyConfigDomainList( "//"OY_TYPE_STD"/config.device.icc_profile",
                        &texts, &count, &rank_list ,0 );

    for (i = 0; i < count; i++)
    {
      oyConfigs_s * devices = 0;
      const char * reg_app = strrchr(texts[i],'/')+1;
      error = oyDevicesGet( OY_TYPE_STD, reg_app, 0, &devices );
      n = oyConfigs_Count( devices );
      d = oyConfDomain_FromReg( texts[i], 0 );

      // Setup icons for devices parent item.
      if(strstr(reg_app,"monitor"))
        device_icon = QIcon(":/resources/monitor.png");
      else if(strstr(reg_app,"printer"))
        device_icon = QIcon(":/resources/printer.png");
      else if(strstr(reg_app,"scanner"))
        device_icon = QIcon(":/resources/scanner.png");

      // pick the modules device class nick name
      device_class = oyConfDomain_GetText( d, "device_class", oyNAME_NAME );

      QTreeWidgetItem * device_list_sub_tree = new QTreeWidgetItem;
      device_list_sub_tree->setText( ITEM_NAME, device_class );
      deviceListTree->addChild( device_list_sub_tree) ;

      for(j = 0; j < (uint32_t)n; ++j)
      {
        oyConfig_s * device = oyConfigs_Get( devices, j );
        char * device_info = 0;

        // get expensive informations to see the "model" option
        error = oyDeviceGetInfo( device, oyNAME_DESCRIPTION, 0,
                                 &device_info, malloc );
        error = oyDeviceGetInfo( device, oyNAME_NAME, 0,
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

        char * model = strdup(device_item_string.toUtf8());
        if(!model)
        {
          const char * m = oyConfig_FindString( device, "device_name", 0);
          if(m)
            model = strdup(m);
        }

#if 0
        if(device_info)
          device_child->setText(0, QString("\"") + model + "\" "
                                   + QString(device_info));
        else
#endif
        device_child->setText( ITEM_NAME, QString(model) );
        if(model) free(model); model = 0;
        device_child->setIcon(ITEM_NAME, device_icon);
        device_list_sub_tree->addChild(device_child);   

        oyProfile_s * p = 0;
        oyOptions_s * options = 0;
        error = oyOptions_SetFromText( &options,
                                       "//"OY_TYPE_STD"/config/icc_profile.x_color_region_target",
                                       "yes", OY_CREATE_NEW );
        oyDeviceGetProfile( device, options, &p );
        oyOptions_Release( &options );

        if(p)
        {
          QTreeWidgetItem * profile_child = new QTreeWidgetItem;
          const char* profile_name = oyProfile_GetText( p, oyNAME_DESCRIPTION );
          const char* file_name = oyProfile_GetFileName( p, -1 );
          if(file_name)
            profile_child->setText( ITEM_NAME, QString(file_name) );
          else
            profile_child->setText( ITEM_NAME, "in memory" );
          profile_child->setText( ITEM_DESCRIPTION, profile_name );

          // attach the profile to the widget
          QVariant v((qulonglong) oyProfile_Copy(p,0));
          profile_child->setData( 0, Qt::UserRole, v );

          profile_child->setIcon(ITEM_ICON, examineIcon);
          device_child->addChild(profile_child);
        }

        //oyProfile_Release( &p );
        oyConfig_Release( &device );
      }
      oyConfigs_Release( &devices );
      oyConfDomain_Release( &d );
    }
}

// Refresh profile information list.
void SyInfo::populateDeviceProfileDescriptions(oyProfile_s * profile, bool valid)
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

        QString profilePathName = oyProfile_GetFileName( profile, 0 );
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
    }
  
}

void SyInfo::loadProfileGraph()
{
        infoDialog->loadProfileGraph(m_tempFile.fileName());

        m_tempFile.close();
}

// Function to write signature head, based on profile, tag type, and QT Label.
void SyInfo::setTagDescriptions(oyProfile_s * profile_name, icTagSignature tagType, DialogString tag )
{
     int text_n;
     bool error;
     
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


void SyInfo::setPcsTag(oyProfile_s * profile)
{
     QString tagString;
     tagString = oyICCColourSpaceGetName( (icColorSpaceSignature)
                         oyProfile_GetSignature(profile, oySIGNATURE_PCS) );
     infoDialog->setDialogText( PCS_TAG, "CIE" + tagString.toLocal8Bit());
     
}


void SyInfo::setColorSpaceTag(oyProfile_s * profile)
{
     QString tagString;
     tagString = oyICCColourSpaceGetName( (icColorSpaceSignature)
                         oyProfile_GetSignature(profile, oySIGNATURE_COLOUR_SPACE) );
     infoDialog->setDialogText( COLORSPACE_TAG, tagString);     
}


void SyInfo::setIccsTag(oyProfile_s * profile)
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


void SyInfo::setDeviceClassTag(oyProfile_s * profile)
{
     QString string;
     string = oyICCDeviceClassDescription( (icProfileClassSignature)
                         oyProfile_GetSignature(profile, oySIGNATURE_CLASS) );
     infoDialog->setDialogText( DEVICE_TAG, string.toLocal8Bit());     
}


void SyInfo::setDateTag(oyProfile_s * profile)
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
                                 monthString.setNum(month) + "/" 
                                 + dayString.setNum(day) + "/" 
                                 + yearString.setNum(year));
}


SyInfo::~SyInfo()
{
    delete infoDialog;
}
