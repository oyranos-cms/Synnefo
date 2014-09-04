 
#ifndef SY_SETTINGS_H
#define SY_SETTINGS_H

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QSignalMapper>
#include <QListWidgetItem>
#include <QWidget>

#include <oyranos.h>

#include "symodule.h"
#include "sy_settings_config.h"

namespace Ui {
  class sySettingsWidget;
}

class SySettingsModule : public SyModule
{
    Q_OBJECT

public:
    SySettingsModule(QWidget * parent = 0);
    ~SySettingsModule();


private slots:

    // Refreshes which policy is currently highlighted/selected.
    void selectPolicy(QListWidgetItem*);

    // Function used when user clicks on "Add Custom Policy".
    void addNewPolicy();

    // Function to remove custom policy.
    void removeCustomPolicy();

    // Write changes to the custom Xml File.
    void saveCustomXmlFile();

    // Enables "Apply" button.
    void emitChanged();

    // Button is called to save custom default profile into XML file.
    void saveSettingsToXml();

private:

    // Used to fill in "Default Profiles" tab.
    void populateProfiles();

    // Used to fill in "Behavior Settings" tab.
    void populateBehaviorSettings();

    // Refresh profile list with changed policy settings.
    void refreshProfileSettings();
 
    // Refresh policy displaying
    void refreshPolicySettings();

    // Function that handles read-only instances.
    void setEditableItems(bool);

    // Function to 'create' a new xml file.
    void writeNewXmlFile(QString policyname, QString filename);

    // Filter default profiles.
    void fillProfileComboBoxes(oyPROFILE_e, QComboBox *);
 
    // Save policies
    void savePolicy();

    // Load policies
    void loadPolicy();

    // Set up all available widgets that are editable into lists.
    void loadEditableItems();

    // Store the settings in the Oyranos database
    void saveSettings();

 
  // *** PRIVATE DATA MEMBERS ***

    SySettingsConfig * settingsConfig;
  
    // Holds all available policy names on the list.
    QStringList installedCustomPolicies;

    // For items that are editable by the user, having them stored
    // in a list allows us to easily detect if "one" item has been changed.
    QList<QComboBox*> editableComboItems;
    QList<QCheckBox*> editableCheckBoxItems;

    // Strings to store policy names.
    QString selected_policy;
    QString default_policy;

    // String to hold new custom XML filename.
    QString xmlFileName;
   
    // String used to store the path of a recently installed custom policy.
    QString customProfileDirectory;

    // Variable to specify if current policy is a custom one.
    bool isCustom, settingsChanged;
   
    int icc_profile_flags;              ///< profile selection flags from oyProfile_s.h

    Ui::sySettingsWidget * ui;
};

#endif
