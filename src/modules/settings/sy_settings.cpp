#include <QDir>
#include <QFile>
#include <QInputDialog>
 
#include "sy_settings.h"

// Qt Designer code translation.
#include "ui_sy_settings.h"    

#include <oyranos.h>
#include <oyFilterNode_s.h>
#include <oyProfiles_s.h>
#include <oyranos_devices.h>

const char * sy_settings_module_name = "Settings";

SySettingsModule::SySettingsModule(QWidget * parent)
    : SyModule(parent)
{       
    setModuleName(sy_settings_module_name);
    setDescription("Set system-wide profile parameters.");
    
    // select profiles matching actual capabilities
    icc_profile_flags = oyICCProfileSelectionFlagsFromOptions( OY_CMM_STD, "//" OY_TYPE_STD "/icc_color", NULL, 0 );

    ui = new Ui::sySettingsWidget();
    ui->setupUi(this);
    
    SySettingsConfig * settingsConfig = new SySettingsConfig(0, sy_settings_module_name);
    
    setConfigWidget(settingsConfig);
    
    setEditable(true);
    
    loadPolicy();

    ui->removePolicyButton->setEnabled(false);

    loadEditableItems();        // Store all setting widgets into a convenient list structure.

    populateProfiles();         // Load all Oyranos-specified profiles
                               // into each combobox in the "Default Profiles" tab.

    modified = false;

    /* i18n */
    QString qs;
    int i;
    int             count = 0,
                    current = 0,
                    flags = 0;
    const char    * tooltip = NULL;
    const char   ** names = NULL;
    const char    * name = NULL,
                  * description = NULL;

    oyWidgetTitleGet( oyWIDGET_GROUP_ALL, NULL, &name,
                      NULL, NULL );
    oyWidgetDescriptionGet( oyWIDGET_GROUP_ALL, &description, 0 );
    setModuleName(QString::fromLocal8Bit(name));
    setDescription(QString::fromLocal8Bit(description));
  
    oyWidgetTitleGet( oyWIDGET_POLICY_ACTIVE, NULL, &name, &tooltip, &flags );
    qs = QString::fromLocal8Bit(name);
    ui->currentPolicyTitleLabel->setText(qs);

    oyWidgetTitleGet( oyWIDGET_POLICY, NULL, &name, &tooltip, &flags );
    qs = QString::fromLocal8Bit(name);
    ui->policySettingsBox->setTitle(qs);
    ui->kmsettingsTab->setTabText(0,qs);
    qs = QString::fromLocal8Bit( tooltip );
    ui->policyLabel->setText(qs);

    oyWidgetTitleGet( oyWIDGET_GROUP_DEFAULT_PROFILES, NULL, &name, &tooltip, &flags );
    qs = QString::fromLocal8Bit(name);
    ui->kmsettingsTab->setTabText(1,qs);
  
    oyWidgetTitleGet( oyWIDGET_GROUP_BEHAVIOUR, NULL, &name, &tooltip, &flags );
    qs = QString::fromLocal8Bit(name);
    ui->kmsettingsTab->setTabText(2,qs);

#define SET_OY_PROFILE_WIDGET( widget ) \
    oyWidgetTitleGet( oyWIDGET_##widget, NULL, &name, &tooltip, &flags ); \
    qs = QString::fromLocal8Bit(name); \
    ui->label_##widget->setText( qs ); \
    qs = QString::fromLocal8Bit( tooltip ); \
    ui->label_##widget->setToolTip( qs ); \
    ui->combo_##widget->setToolTip( qs );

#define SET_OY_BOX_WIDGET( widget ) \
    oyWidgetTitleGet( oyWIDGET_##widget, NULL, &name, &tooltip, &flags ); \
    qs = QString::fromLocal8Bit(name); \
    ui->box_##widget->setTitle( qs ); \
    qs = QString::fromLocal8Bit( tooltip ); \
    ui->box_##widget->setToolTip( qs );

#define SET_OY_COMBO_WIDGET( widget ) \
    oyWidgetTitleGet( oyWIDGET_##widget, NULL, &name, &tooltip, &flags ); \
    oyOptionChoicesGet( oyWIDGET_##widget, &count, &names, &current ); \
    qs = QString::fromLocal8Bit( tooltip ); \
    ui->combo_##widget->setToolTip( qs ); \
    ui->combo_##widget->clear(); \
    for(i = 0; i < count; ++i) \
    { \
      qs = QString::fromLocal8Bit( names[i] ); \
      ui->combo_##widget->addItem( qs ); \
    }

#define SET_OY_CHECK_WIDGET( widget ) \
    oyWidgetTitleGet( oyWIDGET_##widget, NULL, &name, &tooltip, &flags ); \
    qs = QString::fromLocal8Bit( name ); \
    ui->check_##widget->setText( qs ); \
    qs = QString::fromLocal8Bit( tooltip ); \
    ui->check_##widget->setToolTip( qs );

#define SET_OY_LABEL_WIDGET( widget ) \
    oyWidgetTitleGet( oyWIDGET_##widget, NULL, &name, &tooltip, &flags ); \
    qs = QString::fromLocal8Bit(name); \
    ui->label_##widget->setText( qs ); \
    qs = QString::fromLocal8Bit( tooltip ); \
    ui->label_##widget->setToolTip( qs );

    SET_OY_PROFILE_WIDGET( EDITING_RGB );
    SET_OY_PROFILE_WIDGET( EDITING_CMYK );
    SET_OY_PROFILE_WIDGET( EDITING_LAB );
    SET_OY_PROFILE_WIDGET( EDITING_XYZ );
    SET_OY_PROFILE_WIDGET( EDITING_GRAY );
    SET_OY_PROFILE_WIDGET( ASSUMED_RGB );
    SET_OY_PROFILE_WIDGET( ASSUMED_CMYK );
    SET_OY_PROFILE_WIDGET( ASSUMED_LAB );
    SET_OY_PROFILE_WIDGET( ASSUMED_XYZ );
    SET_OY_PROFILE_WIDGET( ASSUMED_GRAY );
    SET_OY_PROFILE_WIDGET( ASSUMED_WEB );
    SET_OY_PROFILE_WIDGET( PROFILE_PROOF );
    SET_OY_BOX_WIDGET( GROUP_BEHAVIOUR_RENDERING );
    SET_OY_BOX_WIDGET( GROUP_BEHAVIOUR_MIXED_MODE_DOCUMENTS );
    SET_OY_BOX_WIDGET( GROUP_BEHAVIOUR_MISSMATCH );
    SET_OY_BOX_WIDGET( GROUP_BEHAVIOUR_PROOF );
    SET_OY_COMBO_WIDGET( ACTION_UNTAGGED_ASSIGN );
    SET_OY_LABEL_WIDGET( ACTION_UNTAGGED_ASSIGN );
    SET_OY_COMBO_WIDGET( ACTION_OPEN_MISMATCH_RGB );
    SET_OY_LABEL_WIDGET( ACTION_OPEN_MISMATCH_RGB );
    SET_OY_COMBO_WIDGET( ACTION_OPEN_MISMATCH_CMYK );
    SET_OY_LABEL_WIDGET( ACTION_OPEN_MISMATCH_CMYK );
    SET_OY_COMBO_WIDGET( MIXED_MOD_DOCUMENTS_PRINT );
    SET_OY_LABEL_WIDGET( MIXED_MOD_DOCUMENTS_PRINT );
    SET_OY_COMBO_WIDGET( MIXED_MOD_DOCUMENTS_SCREEN );
    SET_OY_LABEL_WIDGET( MIXED_MOD_DOCUMENTS_SCREEN );
    SET_OY_COMBO_WIDGET( RENDERING_INTENT );
    SET_OY_COMBO_WIDGET( RENDERING_INTENT_PROOF );
    SET_OY_LABEL_WIDGET( RENDERING_INTENT_PROOF );
    SET_OY_CHECK_WIDGET( RENDERING_BPC );
    SET_OY_CHECK_WIDGET( PROOF_SOFT );
    SET_OY_CHECK_WIDGET( PROOF_HARD );
    SET_OY_CHECK_WIDGET( RENDERING_GAMUT_WARNING );

    // Load behavior settings and display current default policy.
    populateBehaviorSettings();
    refreshProfileSettings();         // Refresh comboboxes in "Default Profiles"
    refreshPolicySettings();

    // QT-related SIGNAL/SLOT functions, such as button presses and clicking
    // on a particular item.
    connect(ui->policySettingsList, SIGNAL(itemClicked(QListWidgetItem*)),
         this, SLOT(selectPolicy(QListWidgetItem*)));
    connect(ui->currentPolicyCombo, SIGNAL(activated(int)),
         this, SLOT(selectPolicy(int)));
    connect(ui->addNewPolicyButton, SIGNAL(clicked()), this, SLOT(addNewPolicy()));
    connect(ui->removePolicyButton, SIGNAL(clicked()), this, SLOT(removeCustomPolicy()));

   int k = 0, n = editableComboItems.size();
   // When a user clicks on any combo box, the "Apply" button will be enabled.
   for(k = 0; k < n; ++k)
    connect(editableComboItems.value(k), SIGNAL(activated(int)), this, SLOT(emitChanged()));

   n = editableCheckBoxItems.size();
   // When a user clicks on a radio box, the "Apply" button will be enabled.
   for(k = 0; k < n; ++k)
    connect(editableCheckBoxItems.value(k), SIGNAL(clicked()), this, SLOT(emitChanged()));
}


// ******* SIGNAL/SLOT Functions *******


void SySettingsModule::selectPolicy(QListWidgetItem* selectedPolicyItem)
{
    // If user makes a settings change, and then clicks on a different policy...
    if (modified == true && isCustom == true)   
    {
        /*if(KMessageBox::questionYesNo(this,
        i18n("The policy settings for '" + selected_policy.toLocal8Bit() + "' have changed. \nDo you wish to save them?"),
        i18n("Policy settings have been modified...")) == KMessageBox::Yes)*/
            
            save();

            modified = false;
            setModified(false);
     }

     selected_policy = selectedPolicyItem->text();

     if(!selectedPolicyItem)
       return;

     checkPolicy(1);
}

// Selected policy from combo box by the user.
void SySettingsModule::selectPolicy(int current)
{
     selected_policy = ui->currentPolicyCombo->currentText();

     if(!selected_policy.count())
       return;

     oyPolicySet( selected_policy.toLocal8Bit(), 0 );

     checkPolicy(0);
}


void SySettingsModule::addNewPolicy()
{   
    bool ok;
   
    // New policy name dialog.
    xmlFileName = QInputDialog::getText(this, tr("Add new policy"), 
                       tr("Please enter a name for the new policy: "), 
                       QLineEdit::Normal, QDir::home().dirName(), &ok);
 
    if(ok)   
    {
        int i;
        QListWidgetItem * temp_item;

        for(i = 0; i < ui->policySettingsList->count(); i++)
        {
             temp_item = ui->policySettingsList->item(i);
             if (xmlFileName == temp_item->text())
             {
                  // TODO Display "Policy exists" Error message.
                
                   return;
             }
        }    
       
        ui->policySettingsList->addItem(xmlFileName);     // Add policy name to list.
    }
    else 
       return;
  
    savePolicy();

    // Create a "blank" XML file based on entered name.
    selected_policy = xmlFileName;
    saveCustomXmlFile();
    refreshPolicySettings();
}


void SySettingsModule::removeCustomPolicy()
{
    // Remove policy from list file.
    QListWidgetItem * deleted_item = ui->policySettingsList->takeItem(ui->policySettingsList->currentRow());
    savePolicy();

    // Remove actual Xml file from directory.
    char * full_name = 0;
    oyPolicyFileNameGet( deleted_item->text().toLocal8Bit(),&full_name,malloc);
    QFile file( full_name );
    file.remove();
    if(full_name) free( full_name );
}



// ****** Private Functions ******


//  Fill the combo boxes in the "Default Profiles" tab with Oyranos-installed profiles.
void SySettingsModule::populateProfiles()
{
    // Fill comboboxes with each 'filtered' profile.
    fillProfileComboBoxes(oyEDITING_RGB, ui->combo_EDITING_RGB);
    fillProfileComboBoxes(oyEDITING_CMYK, ui->combo_EDITING_CMYK);
    fillProfileComboBoxes(oyEDITING_LAB, ui->combo_EDITING_LAB);
    fillProfileComboBoxes(oyEDITING_XYZ, ui->combo_EDITING_XYZ);
    fillProfileComboBoxes(oyEDITING_GRAY, ui->combo_EDITING_GRAY);

    fillProfileComboBoxes(oyASSUMED_RGB, ui->combo_ASSUMED_RGB);
    fillProfileComboBoxes(oyASSUMED_CMYK, ui->combo_ASSUMED_CMYK);
    fillProfileComboBoxes(oyASSUMED_LAB, ui->combo_ASSUMED_LAB);
    fillProfileComboBoxes(oyASSUMED_XYZ, ui->combo_ASSUMED_XYZ);
    fillProfileComboBoxes(oyASSUMED_GRAY, ui->combo_ASSUMED_GRAY);  

    fillProfileComboBoxes(oyPROFILE_PROOF, ui->combo_PROFILE_PROOF);
}


// Populate each setting in "Behavior Settings".
void SySettingsModule::populateBehaviorSettings()
{ 
    int behavior_setting;

    behavior_setting = oyGetBehaviour(oyBEHAVIOUR_RENDERING_INTENT);
    ui->combo_RENDERING_INTENT->setCurrentIndex(behavior_setting);

//  Populate Mismatch Handling Settings
   
    behavior_setting = oyGetBehaviour(oyBEHAVIOUR_ACTION_UNTAGGED_ASSIGN);
    ui->combo_ACTION_UNTAGGED_ASSIGN->setCurrentIndex(behavior_setting);

    behavior_setting = oyGetBehaviour(oyBEHAVIOUR_ACTION_OPEN_MISMATCH_RGB);
    ui->combo_ACTION_OPEN_MISMATCH_RGB->setCurrentIndex(behavior_setting);

    behavior_setting = oyGetBehaviour(oyBEHAVIOUR_ACTION_OPEN_MISMATCH_CMYK);
    ui->combo_ACTION_OPEN_MISMATCH_CMYK->setCurrentIndex(behavior_setting);

//  Set up Proofing Settings  
  
    behavior_setting = oyGetBehaviour(oyBEHAVIOUR_RENDERING_INTENT_PROOF);
    ui->combo_RENDERING_INTENT_PROOF->setCurrentIndex(behavior_setting);

    behavior_setting = oyGetBehaviour(oyBEHAVIOUR_RENDERING_BPC);
    if(behavior_setting == 1)
         ui->check_RENDERING_BPC->setChecked(true);
    else 
         ui->check_RENDERING_BPC->setChecked(false);

    behavior_setting = oyGetBehaviour(oyBEHAVIOUR_RENDERING_GAMUT_WARNING);
    if(behavior_setting == 1)
         ui->check_RENDERING_GAMUT_WARNING->setChecked(true);
    else 
         ui->check_RENDERING_GAMUT_WARNING->setChecked(false);

    behavior_setting = oyGetBehaviour(oyBEHAVIOUR_PROOF_SOFT);
    if(behavior_setting == 1)
         ui->check_PROOF_SOFT->setChecked(true);
    else 
         ui->check_PROOF_SOFT->setChecked(false);

    behavior_setting = oyGetBehaviour(oyBEHAVIOUR_PROOF_HARD);

    if(behavior_setting == 1)
         ui->check_PROOF_HARD->setChecked(true);
    else 
         ui->check_PROOF_HARD->setChecked(false);

// Set up Mixed Color Settings

    behavior_setting = oyGetBehaviour(oyBEHAVIOUR_MIXED_MOD_DOCUMENTS_SCREEN);
    ui->combo_MIXED_MOD_DOCUMENTS_SCREEN->setCurrentIndex(behavior_setting);

    behavior_setting = oyGetBehaviour(oyBEHAVIOUR_MIXED_MOD_DOCUMENTS_PRINT);
    ui->combo_MIXED_MOD_DOCUMENTS_PRINT->setCurrentIndex(behavior_setting);     
}


//  Refresh profile combo boxes with profiles associated with the current policy.
void SySettingsModule::refreshProfileSettings()
{
    int profileSearchIndex;  // Index to match profile stored in a combobox.

    QString xmlToString;     // String used to grab each default colorspace
                             // profile from Oyranos.

    /* Each item will grab the default profile, and match it with an available profile
       under each combo box in "Default Profiles".  The combobox will then display
       the default profile.                                                     */
    xmlToString = oyGetDefaultProfileName (oyEDITING_RGB, 0); 
    profileSearchIndex = ui->combo_EDITING_RGB->findText( xmlToString, Qt::MatchExactly);
    ui->combo_EDITING_RGB->setCurrentIndex(profileSearchIndex);
     
    xmlToString = oyGetDefaultProfileName (oyEDITING_CMYK, 0);
    profileSearchIndex = ui->combo_EDITING_CMYK->findText( xmlToString, Qt::MatchExactly);
    ui->combo_EDITING_CMYK->setCurrentIndex(profileSearchIndex);
     
    xmlToString = oyGetDefaultProfileName (oyEDITING_XYZ, 0);
    profileSearchIndex = ui->combo_EDITING_XYZ->findText( xmlToString, Qt::MatchExactly);
    ui->combo_EDITING_XYZ->setCurrentIndex(profileSearchIndex);
    
    xmlToString = oyGetDefaultProfileName (oyEDITING_LAB, 0);
    profileSearchIndex = ui->combo_EDITING_LAB->findText( xmlToString, Qt::MatchExactly);
    ui->combo_EDITING_LAB->setCurrentIndex(profileSearchIndex);

    xmlToString = oyGetDefaultProfileName (oyEDITING_GRAY, 0);
    profileSearchIndex = ui->combo_EDITING_GRAY->findText( xmlToString, Qt::MatchExactly);
    ui->combo_EDITING_GRAY->setCurrentIndex(profileSearchIndex);  
     
    xmlToString = oyGetDefaultProfileName (oyASSUMED_RGB, 0);
    profileSearchIndex = ui->combo_ASSUMED_RGB->findText( xmlToString, Qt::MatchExactly);
    ui->combo_ASSUMED_RGB->setCurrentIndex(profileSearchIndex);
     
    xmlToString = oyGetDefaultProfileName (oyASSUMED_CMYK, 0);
    profileSearchIndex = ui->combo_EDITING_CMYK->findText( xmlToString, Qt::MatchExactly);
    ui->combo_ASSUMED_CMYK->setCurrentIndex(profileSearchIndex);
     
    xmlToString = oyGetDefaultProfileName (oyASSUMED_XYZ, 0);
    profileSearchIndex = ui->combo_EDITING_XYZ->findText( xmlToString, Qt::MatchExactly);
    ui->combo_ASSUMED_XYZ->setCurrentIndex(profileSearchIndex);
    
    xmlToString = oyGetDefaultProfileName (oyASSUMED_LAB, 0);
    profileSearchIndex = ui->combo_ASSUMED_LAB->findText( xmlToString, Qt::MatchExactly);
    ui->combo_ASSUMED_LAB->setCurrentIndex(profileSearchIndex);  

    xmlToString = oyGetDefaultProfileName (oyASSUMED_GRAY, 0);
    profileSearchIndex = ui->combo_ASSUMED_GRAY->findText( xmlToString, Qt::MatchExactly);
    ui->combo_ASSUMED_GRAY->setCurrentIndex(profileSearchIndex);  

    xmlToString = oyGetDefaultProfileName (oyPROFILE_PROOF, 0);
    profileSearchIndex = ui->combo_PROFILE_PROOF->findText( xmlToString, Qt::MatchExactly);
    ui->combo_PROFILE_PROOF->setCurrentIndex(profileSearchIndex);    
}


void SySettingsModule::refreshPolicySettings()
{
    // Load policy.
    int count = 0, current = -1;
    const char ** names = 0;
    oyOptionChoicesGet( oyWIDGET_POLICY, &count, &names, &current );

    ui->currentPolicyCombo->clear();
    for(int i = 0; i < count; ++i)
      ui->currentPolicyCombo->insertItem(i, names[i], 0);

    ui->currentPolicyCombo->setCurrentIndex(-1);
    if(names && count && current >= 0)
    {
      selected_policy = names[current];
      // Set user selected policy as system default.
      ui->currentPolicyCombo->setCurrentIndex(current);
    }
}


// This function controls whether or not the settings are read-only.
void SySettingsModule::setEditableItems(bool itemStatus)
{   
    QComboBox * combobox;
    for (int i = 0; i < editableComboItems.size(); i++)
    {
        combobox = editableComboItems.value(i);
        combobox->setEnabled(itemStatus);
        connect(combobox, SIGNAL(activated(int)), this, SLOT(emitChanged()));
    }

    QCheckBox * checkbox;
    for (int i = 0; i < editableCheckBoxItems.size(); i++)
    {
        checkbox = editableCheckBoxItems.value(i);
        checkbox->setEnabled(itemStatus);
        connect(checkbox, SIGNAL(clicked()), this, SLOT(emitChanged()));
    }
}


// Load editable comboboxe and checkbox items into a list
//  (this is convenient to detect each settings change by the user).
void SySettingsModule::loadEditableItems()
{      
    editableComboItems.push_front(ui->combo_EDITING_RGB);
    editableComboItems.push_front(ui->combo_EDITING_CMYK);   
    editableComboItems.push_front(ui->combo_EDITING_XYZ);
    editableComboItems.push_front(ui->combo_EDITING_LAB);
    editableComboItems.push_front(ui->combo_ASSUMED_RGB);
    editableComboItems.push_front(ui->combo_ASSUMED_CMYK);   
    editableComboItems.push_front(ui->combo_ASSUMED_XYZ);
    editableComboItems.push_front(ui->combo_ASSUMED_LAB);

    editableComboItems.push_front(ui->combo_EDITING_GRAY);
    editableComboItems.push_front(ui->combo_ASSUMED_GRAY);   
     
    editableComboItems.push_front(ui->combo_RENDERING_INTENT);
    editableComboItems.push_front(ui->combo_ACTION_UNTAGGED_ASSIGN);    
    editableComboItems.push_front(ui->combo_ACTION_OPEN_MISMATCH_RGB);
    editableComboItems.push_front(ui->combo_ACTION_OPEN_MISMATCH_CMYK);
    editableComboItems.push_front(ui->combo_RENDERING_INTENT_PROOF);    
    editableComboItems.push_front(ui->combo_MIXED_MOD_DOCUMENTS_SCREEN);
    editableComboItems.push_front(ui->combo_MIXED_MOD_DOCUMENTS_PRINT);     
    editableComboItems.push_front(ui->combo_PROFILE_PROOF);

    editableCheckBoxItems.push_front(ui->check_PROOF_SOFT);
    editableCheckBoxItems.push_front(ui->check_PROOF_HARD);
    editableCheckBoxItems.push_front(ui->check_RENDERING_BPC);
    editableCheckBoxItems.push_front(ui->check_RENDERING_GAMUT_WARNING);
     
}


// Filter comboboxes in 'Default Profiles' with appropriate profiles.
void SySettingsModule::fillProfileComboBoxes(unsigned int type, QComboBox * profile_combobox)
{
    int current = 0, size, i,
                  sizeofProfilename,
                  indexofProfilename;
    oyProfile_s * temp_prof = 0;
    oyProfiles_s * iccs = 0;
    oyPROFILE_e profile_type = (oyPROFILE_e) type;

    QString profile_text, profile_filename;

    iccs = oyProfiles_ForStd( profile_type, icc_profile_flags, &current, 0 );

    size = oyProfiles_Count(iccs);
    for( i = 0; i < size; i++)
    {
       temp_prof = oyProfiles_Get( iccs, i );
       profile_text = oyProfile_GetText( temp_prof, oyNAME_DESCRIPTION );
       profile_filename = oyProfile_GetFileName(temp_prof, 0);

       sizeofProfilename =  profile_filename.size();
       indexofProfilename = profile_filename.lastIndexOf( "/", sizeofProfilename - 1);
       profile_filename.remove(0, indexofProfilename + 1);

       profile_combobox->insertItem(i, profile_filename, 0);

       oyProfile_Release( &temp_prof );
    }
}


void SySettingsModule::saveCustomXmlFile()
{ 
    saveSettings();
    oySCOPE_e scope = ui->systemWideCheckBox->isChecked()
                      ? oySCOPE_SYSTEM : oySCOPE_USER;
    oyPolicySaveActual(oyGROUP_ALL, scope, selected_policy.toLocal8Bit()); 
}


/*
   When the user wants to add a custom policy, 
   the program will prompt user for new policy name 
   and add to the installed policies list.      */





void SySettingsModule::saveSettingsToXml()
{     
   saveCustomXmlFile();
   refreshPolicySettings();
}


void SySettingsModule::saveSettings()
{ 
    QString stringToXml;    
    int behaviorSetting;    
    oySCOPE_e scope = ui->systemWideCheckBox->isChecked()
                      ? oySCOPE_SYSTEM : oySCOPE_USER;

    stringToXml = ui->combo_EDITING_RGB->currentText();
    oySetDefaultProfile(oyEDITING_RGB, scope, stringToXml.toLocal8Bit()); 
 
    stringToXml = ui->combo_EDITING_CMYK->currentText();
    oySetDefaultProfile(oyEDITING_CMYK, scope, stringToXml.toLocal8Bit());

    stringToXml = ui->combo_EDITING_XYZ->currentText();
    oySetDefaultProfile(oyEDITING_XYZ, scope, stringToXml.toLocal8Bit());

    stringToXml = ui->combo_EDITING_LAB->currentText();
    oySetDefaultProfile(oyEDITING_LAB, scope, stringToXml.toLocal8Bit());

    stringToXml = ui->combo_EDITING_GRAY->currentText();
    oySetDefaultProfile(oyEDITING_GRAY, scope, stringToXml.toLocal8Bit());

    stringToXml = ui->combo_ASSUMED_RGB->currentText();
    oySetDefaultProfile(oyASSUMED_RGB, scope, stringToXml.toLocal8Bit());

    stringToXml = ui->combo_ASSUMED_CMYK->currentText();
    oySetDefaultProfile(oyASSUMED_CMYK, scope, stringToXml.toLocal8Bit());

    stringToXml = ui->combo_ASSUMED_LAB->currentText();
    oySetDefaultProfile(oyASSUMED_LAB, scope, stringToXml.toLocal8Bit());

    stringToXml = ui->combo_ASSUMED_XYZ->currentText();
    oySetDefaultProfile(oyASSUMED_XYZ, scope, stringToXml.toLocal8Bit());

    stringToXml = ui->combo_ASSUMED_GRAY->currentText();
    oySetDefaultProfile(oyASSUMED_GRAY, scope, stringToXml.toLocal8Bit());

    stringToXml = ui->combo_PROFILE_PROOF->currentText();
    oySetDefaultProfile(oyPROFILE_PROOF, scope, stringToXml.toLocal8Bit());

    //----------------------------------------------------------------

    behaviorSetting = ui->combo_RENDERING_INTENT->currentIndex();
    oySetBehaviour ( oyBEHAVIOUR_RENDERING_INTENT, scope, behaviorSetting );

    behaviorSetting = ui->combo_ACTION_UNTAGGED_ASSIGN->currentIndex();
    oySetBehaviour ( oyBEHAVIOUR_ACTION_UNTAGGED_ASSIGN, scope, behaviorSetting );

    behaviorSetting = ui->combo_ACTION_OPEN_MISMATCH_RGB->currentIndex();
    oySetBehaviour ( oyBEHAVIOUR_ACTION_OPEN_MISMATCH_RGB, scope , behaviorSetting );
  
    behaviorSetting = ui->combo_ACTION_OPEN_MISMATCH_CMYK->currentIndex();
    oySetBehaviour ( oyBEHAVIOUR_ACTION_OPEN_MISMATCH_CMYK, scope , behaviorSetting );

    behaviorSetting = ui->combo_RENDERING_INTENT_PROOF->currentIndex();
    oySetBehaviour ( oyBEHAVIOUR_RENDERING_INTENT_PROOF, scope , behaviorSetting );

    behaviorSetting = ui->combo_MIXED_MOD_DOCUMENTS_SCREEN->currentIndex();
    oySetBehaviour ( oyBEHAVIOUR_MIXED_MOD_DOCUMENTS_SCREEN, scope , behaviorSetting );

    behaviorSetting = ui->combo_MIXED_MOD_DOCUMENTS_PRINT->currentIndex();
    oySetBehaviour ( oyBEHAVIOUR_MIXED_MOD_DOCUMENTS_PRINT, scope , behaviorSetting );

    if (ui->check_RENDERING_BPC->isChecked())
        oySetBehaviour ( oyBEHAVIOUR_RENDERING_BPC, scope , 1 );
    else
        oySetBehaviour ( oyBEHAVIOUR_RENDERING_BPC, scope , 0 );

    if (ui->check_RENDERING_GAMUT_WARNING->isChecked())
        oySetBehaviour ( oyBEHAVIOUR_RENDERING_GAMUT_WARNING, scope , 1 );
    else
        oySetBehaviour ( oyBEHAVIOUR_RENDERING_GAMUT_WARNING, scope , 0 );

    if (ui->check_PROOF_SOFT->isChecked())
        oySetBehaviour ( oyBEHAVIOUR_PROOF_SOFT, scope , 1 );
    else
        oySetBehaviour ( oyBEHAVIOUR_PROOF_SOFT, scope , 0 );

    if (ui->check_PROOF_HARD->isChecked())
        oySetBehaviour( oyBEHAVIOUR_PROOF_HARD, scope , 1 );
    else 
        oySetBehaviour(oyBEHAVIOUR_PROOF_HARD , scope , 0 );
}
// Create a new file that's currently stored in the customProfileDirectory QString.



// Used to enable the "Apply" button.
void SySettingsModule::emitChanged()
{
     // Ungrey "Apply" button. 
     modified = true;
     setModified(true);     

     saveSettings();
     refreshPolicySettings();
}





// Function to save/refresh installed policies in system.
void SySettingsModule::savePolicy()
{
     QString tempProfile;
     QStringList policyList;

     if ( ui->policySettingsList->count() >= 4)
     {
         QListWidgetItem * temp_item;

         for (int i = 4; i < ui->policySettingsList->count(); i++)
         {
              temp_item = ui->policySettingsList->item(i);
              tempProfile = temp_item->text();

              policyList.insert(0, tempProfile);
         }
     }
}

// check if the current policy is editable
void SySettingsModule::checkPolicy(int set)
{
     if(!selected_policy.count())
       return;

     char * full_name = 0;
     oyPolicyFileNameGet( selected_policy.toLocal8Bit(), &full_name, malloc );
     QFile file( full_name );
     if(file.permissions() & QFile::WriteOwner)
     {
         isCustom = true;             // This is a custom policy.
         
         ui->removePolicyButton->setEnabled(true);

         if(set)
           oyPolicySet( selected_policy.toLocal8Bit(), 0 );
     }
     if(full_name) free( full_name );

   
     // Make sure the user doesn't delete the current policy settings!
     if(default_policy == selected_policy)
         ui->removePolicyButton->setEnabled(false);


     for (int i = 0; i < ui->policySettingsList->count(); i++)
     {
       QListWidgetItem * temp_item = ui->policySettingsList->item(i);
       if(selected_policy == temp_item->text())
         ui->policySettingsList->setCurrentRow(i);
     }

     populateBehaviorSettings();       // Refresh settings in "Behavior Settings"
     refreshProfileSettings();         // Refresh comboboxes in "Default Profiles"
     refreshPolicySettings();

     setEditableItems(isCustom);
}


// Function to load the installed policies in system.
void SySettingsModule::loadPolicy()
{
  const char ** names = NULL;
  int count = 0, i, current = -1;

  ui->policySettingsList->clear();

  oyOptionChoicesGet( oyWIDGET_POLICY, &count, &names, &current );
  for(i = 0; i < count; ++i)
    ui->policySettingsList->addItem( names[i] );
}

SySettingsModule::~SySettingsModule()
{
  
}
