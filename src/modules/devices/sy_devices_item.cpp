#include "sy_devices_item.h"


SyDevicesItem::SyDevicesItem( QTreeWidget * parent )
    : QTreeWidgetItem( parent, 0 )
{
    QString deviceDescription = " ";
    QString deviceName = " ";
    QString profileDescription = " ";
    QString profileFilename = " ";
    
    deviceItemText = " ";
}

void  SyDevicesItem::addText( ItemText e, QString appendedText )
{ 
    if (appendedText.isEmpty() == false) 
    {
       switch (e) 
       {
         case DEVICE_DESCRIPTION:
           deviceDescription = appendedText;
           break;
         case DEVICE_NAME:
           deviceName = appendedText;
           break;
         case PROFILE_DESCRIPTION:
           profileDescription = appendedText;
           break;
         case PROFILE_FILENAME:
           profileFilename = appendedText;
           break;
         default:
          return;
       }     
    }
    
    refreshText();
    
    return;
}


QString  SyDevicesItem::getText(ItemText e)
{
    QString text;
    
    switch (e)
    {
      case DEVICE_DESCRIPTION:
         text = deviceDescription;
         break;
      case DEVICE_NAME:
         text = deviceName;
         break;
      case PROFILE_DESCRIPTION:
         text = profileDescription;
         break;
      case PROFILE_FILENAME:
         text = profileFilename;
         break;
    }
    
    return text;
}


void  SyDevicesItem::refreshText()
{
    deviceItemText =  deviceName + 
                     "\n\nDescription: " + deviceDescription +
                     "\nProfile Name: " + profileFilename +
                     "\nProfile description: " + profileDescription;
		     
    this->setText (0, deviceItemText);
}