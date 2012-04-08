
#include "sy_database.h"

#include <oyranos.h>
#include <oyranos_icc.h>
#include <alpha/oyranos_alpha.h>
#include <alpha/oyranos_cmm.h>

const char * sy_database_module_name = "Database";

SyDatabase::SyDatabase(QWidget * parent)
    : SyModule(parent)
{   
    setModuleName(sy_database_module_name);
    setDescription("Browse profiles listed from the Taxi server.");
    
    SyDatabaseConfig * databaseConfig = new SyDatabaseConfig(0, sy_database_module_name);
    setConfigWidget( databaseConfig );
    
    setupUi(this);
    
    populateTaxiProfileList();
}


int SyDatabase::populateTaxiProfileList()
{

}



SyDatabase::~SyDatabase()
{
  
}