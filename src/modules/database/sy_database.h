
#include "symodule.h"

// Qt Designer code translation.
#include "ui_sy_database.h"   
#include "sy_database_config.h"



/*****************************************************************
          Synnefo Database Module Class Definition               *
******************************************************************/



class SyDatabase : public SyModule, Ui::syDatabaseWidget
{
    Q_OBJECT
    
public:
    SyDatabase(QWidget * parent);
    ~SyDatabase();
    
private:
    int populateTaxiProfileList();    
};