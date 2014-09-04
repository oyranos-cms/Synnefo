
#include "sy_database.h"

#include <oyranos.h>
#include <oyranos_icc.h>
#include <oyranos_devices.h>

// Qt Designer code translation.
#include "ui_sy_database.h"   
#include "sy_database_config.h"


const char * sy_database_module_name = "Database";

SyDatabase::SyDatabase(QWidget * parent)
    : SyModule(parent)
{   
    setModuleName(sy_database_module_name);
    setDescription("Browse profiles listed from the Taxi server.");
    
    SyDatabaseConfig * databaseConfig = new SyDatabaseConfig(0, sy_database_module_name);
    setConfigWidget( databaseConfig );

    ui = new Ui::syDatabaseWidget();
    ui->setupUi(this);
    
    populateTaxiProfileList();
}


int SyDatabase::populateTaxiProfileList()
{
  return 0;
}



SyDatabase::~SyDatabase()
{
  delete ui;
}
