#include "sy_info_dialog.h"

SyInfoDialog::SyInfoDialog(QWidget * parent) 
    : QDialog(parent, 0)
{
    setupUi(this);

}
    
void SyInfoDialog::setDialogText( DialogString e, QString text )
{
    switch (e)
    {
      case COLORSPACE_TAG:
        colorspaceTagLabel->setText( text );
        break;
      case COPYRIGHT_TAG:
        copyrightTagLabel->setText( text );
        break;
      case DATE_CREATED_TAG:
        dateTagLabel->setText( text );
        break;
      case DESCRIPTION_TAG:
        descriptionTagLabel->setText( text );
        break;
      case DEVICE_TAG:
        deviceClassTagLabel->setText( text );
        break;
      case FILENAME_TAG:
        filenameTagLabel->setText( text );
        break;
      case ICC_VERSION_TAG:
        iccVerTagLabel->setText( text );
        break;
      case MANUFACTURER_TAG:
        mfgTagLabel->setText( text );
        break;
      case MODEL_TAG:
        modelTagLabel->setText( text );
        break;
      case PCS_TAG:
        pcsTagLabel->setText( text );
        break;
      case PROFILE_PATH_TAG:
        directoryListingTag->setText( text );
        break;
    }
    
    return;
}