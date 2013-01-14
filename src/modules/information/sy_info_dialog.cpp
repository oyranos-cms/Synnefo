
#include <QFileInfo>
#include <oyranos.h>
#include <oyranos_config.h>
#include <alpha/oyranos_alpha.h>

#include "sy_info_dialog.h"

SyInfoDialog::SyInfoDialog(QWidget * parent) 
    : QDialog(parent, 0)
{
    setupUi(this);

    connect( closeButton, SIGNAL( clicked() ), this, SLOT( closeDialog() ));
    connect( launchICCExaminButton, SIGNAL(clicked()), this, SLOT(launchICCExamin())); 
}


/*    SLOTS    */

void  SyInfoDialog::closeDialog ()
{
     this->close();
}


void SyInfoDialog::launchICCExamin()
{

    QString exec;
    
    // NOTE: This needs double-checking.
    oyProfile_s * p = (oyProfile_s *) descriptionTagLabel->text().toULongLong();
    
    //v.toULongLong()

    if(!directoryListingTag->text().isNull())
      exec = iccExaminCommand + " -g \"" + directoryListingTag->text() + "\"&";
    else
    {
      // Write to a temporary file.
      size_t size = 0;
      oyPointer data = oyProfile_GetMem( p, &size, 0, malloc );
      if(data)
      {        
        // The disadvantage is, any previous ICC Examin session for the same
        // file name will update its view to the newly written one.
        QFile file("/tmp/icc_examin_temp.icc");
        file.open( QIODevice::WriteOnly );
        file.write( (const char*)data, size );
        file.flush();
        file.close();
        free(data); data = 0;
        exec = iccExaminCommand + " -g " + "/tmp/icc_examin_temp.icc" + "&";
      } else
        return;
    }
    system(exec.toLocal8Bit());

}

/*    Public Functions    */

    
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


void SyInfoDialog::showDialog()
{   
    this->show(); 
    
    if (iccExaminIsInstalled(iccExaminCommand))
        launchICCExaminButton->show();
    else
        launchICCExaminButton->hide(); 
}


/*    Private Functions    */


// Detect iccexamin for integrated 3D Profile support.
bool SyInfoDialog::iccExaminIsInstalled(QString &iccExaminPath)
{

# ifdef __WIN32__
     QChar pathSep = QChar::fromLatin1(';');
     const QString iccExamin = QString::fromLocal8Bit("iccexamin.exe");
#elif defined (__APPLE__)
     QChar pathSep = QChar::fromLatin1(':'); 
     const QString iccExamin = QString::fromLocal8Bit("iccexamin.app/Contents/MacOS/ICC Examin");
# else
     QChar pathSep = QChar::fromLatin1(':');  
     const QString iccExamin = QString::fromLocal8Bit("iccexamin");
# endif /* __WIN32__ */

     QString Path = QString::fromLocal8Bit(getenv("PATH"));
     QFileInfo fileinfo;

     bool done = false;
     bool found = false;

     while (!done)
     {
          if (!Path.length() > 0) // still more paths to look at
                 done=true;
          else
          {
              // get next path
              iccExaminPath = Path.left(Path.indexOf(pathSep, 0, Qt::CaseSensitive)) +
              QString::fromLocal8Bit("/") + iccExamin ;
              // remove the path we are about to check from path
              Path = Path.right(Path.length() - Path.indexOf(pathSep, 0, Qt::CaseSensitive) - 1);
              // apend executable name to path
              fileinfo.setFile(iccExaminPath);
              // check to see if it exists
             if (fileinfo.exists())
             {  
                 done = true;
                 found = true;
                
             }
             // check to see if this was last path
             if (Path.length() <= iccExaminPath.length())
                 done = true;
           }
     }

     return found;
}


void SyInfoDialog::loadProfileGraph(QString fileName)
{
    QPixmap mypix(fileName);
    launchICCExaminButton->setIcon(QIcon(mypix));
}

