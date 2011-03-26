#ifndef SY_INFO_DIALOG_H
#define SY_INFO_DIALOG_H

#include <QDialog>

#include "ui_sy_info_dialog.h"

enum DialogString {
   COLORSPACE_TAG,
   COPYRIGHT_TAG,
   DATE_CREATED_TAG,
   DESCRIPTION_TAG,
   DEVICE_TAG, 
   FILENAME_TAG,
   ICC_VERSION_TAG,     
   MANUFACTURER_TAG,
   MODEL_TAG,
   PCS_TAG,
   PROFILE_PATH_TAG
};

class SyInfoDialog : public QDialog, Ui::syInfoDialog
{
    Q_OBJECT
    
  public:
    SyInfoDialog(QWidget * parent);
    
    void setDialogText( DialogString e, QString text );
        
};

#endif