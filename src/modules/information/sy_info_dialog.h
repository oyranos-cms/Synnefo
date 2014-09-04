#ifndef SY_INFO_DIALOG_H
#define SY_INFO_DIALOG_H

#include <QDialog>


/* Enumerations used to set tag strings in the dialog. */
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

namespace Ui {
  class syInfoDialog;
}

/*  SyInfoDialog is a class that handles the Profile Information dialog. */
class SyInfoDialog : public QDialog
{
    Q_OBJECT
    
  private slots:
    
    void closeDialog();
    
    // If "Analyze profile" button is un-hidden, launch iccexamin.
    void launchICCExamin();
    
  public:
    SyInfoDialog(QWidget * parent);
    
    void setDialogText( DialogString e, QString text );
    void showDialog();
    void loadProfileGraph( QString fileName );
        
  private:
    
    // Determines if iccExamin is installed on the machine.
    bool iccExaminIsInstalled(QString &iccExaminPath);
    
    QString iccExaminCommand;

    Ui::syInfoDialog * ui;
};

#endif
