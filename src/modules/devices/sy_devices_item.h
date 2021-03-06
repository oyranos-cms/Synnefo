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

struct oyConfig_s;

class SyDevicesItem : public QTreeWidgetItem
{
  public:
    SyDevicesItem (QTreeWidget * parent);
    ~SyDevicesItem();
    void addText (ItemText, QString );
    QString getText (ItemText);
    
    void refreshText();
    void setDevice( oyConfig_s * d );
  private:    
    QString deviceDescription;
    QString deviceName;
    QString profileDescription;
    QString profileFilename;
    QString miscDescription;
    
    QString deviceItemText;
    oyConfig_s * device;
};

#endif 
