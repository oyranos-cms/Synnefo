#ifndef SY_DEVICES_ITEM_H
#define SY_DEVICES_ITEM_H

#include <QListWidgetItem>
#include <QTreeWidgetItem>

 enum ItemText {
      DEVICE_DESCRIPTION,
      DEVICE_NAME,
      PROFILE_DESCRIPTION,
      PROFILE_FILENAME,
      MISC_INFO
    };

/* This class is used within the Synnefo 'Devices' module.
   It extends QListWidgetItem and is used to display 
   information for each device.
   */

class SyDevicesItem : public QTreeWidgetItem
{
  public:
    SyDevicesItem (QTreeWidget * parent);

    void addText (ItemText, QString );
    QString getText (ItemText);
    
    void refreshText();
    
  private:    
    QString deviceDescription;
    QString deviceName;
    QString profileDescription;
    QString profileFilename;
    QString miscDescription;
    
    QString deviceItemText;
};

#endif 