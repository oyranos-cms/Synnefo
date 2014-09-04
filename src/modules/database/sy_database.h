
#include "symodule.h"


/*****************************************************************
          Synnefo Database Module Class Definition               *
******************************************************************/

namespace Ui {
  class syDatabaseWidget;
}

class SyDatabase : public SyModule
{
    Q_OBJECT
    
public:
    SyDatabase(QWidget * parent);
    ~SyDatabase();
    
private:
    int populateTaxiProfileList();    
    Ui::syDatabaseWidget * ui;
};
