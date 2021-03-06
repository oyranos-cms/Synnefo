#include "sy_devices_item.h"

#include <oyranos_devices.h>

SyDevicesItem::SyDevicesItem( QTreeWidget * parent )
    : QTreeWidgetItem( parent, 0 )
{
    deviceDescription = "";
    deviceName = "";
    profileDescription = "";
    profileFilename = "";
    miscDescription = "";
    
    deviceItemText = "";
    device = 0;
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
         case MISC_INFO:
           miscDescription = appendedText;
           break;         
         default:
          return;
       }     
    }
    
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
      case MISC_INFO:
         text = profileFilename;
         break;
    }
    
    return text;
}


void  SyDevicesItem::refreshText()
{
    deviceItemText = deviceName + "\n" + deviceDescription;
    if(miscDescription.size())
      deviceItemText += "\n" + miscDescription;
    this->setText (0, deviceItemText);
}

void SyDevicesItem::setDevice( oyConfig_s * d )
{
  if(device)
    oyConfig_Release( &device );
  device = oyConfig_Copy(d, 0);
}

SyDevicesItem::~SyDevicesItem()
{
  oyConfig_Release( &device );
}
