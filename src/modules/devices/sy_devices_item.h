#ifndef SY_DEVICES_ITEM_H
#define SY_DEVICES_ITEM_H

#include <QListWidgetItem>
#include <QTreeWidgetItem>

 enum ItemText {
      DEVICE_DESCRIPTION,
      DEVICE_NAME,
      PROFILE_DESCRIPTION,
      PROFILE_FILENAME
    };

/* This class is used within the Synnefo 'Devices' module.
   It extends QListWidgetItem and is used to display 
   */

class SyDevicesItem : public QTreeWidgetItem
{
  public:
    SyDevicesItem (QTreeWidget * parent);

    void addText (ItemText, QString );
    QString getText (ItemText);
    
  private:
    void refreshText();
    
    QString deviceDescription;
    QString deviceName;
    QString profileDescription;
    QString profileFilename;
    
    QString deviceItemText;
};

#endif 