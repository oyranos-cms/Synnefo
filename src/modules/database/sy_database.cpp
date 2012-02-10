
#include "sy_database.h"

#include <oyranos.h>
#include <oyranos_icc.h>
#include <alpha/oyranos_alpha.h>
#include <alpha/oyranos_cmm.h>

#include <stdio.h>



SyDatabase::SyDatabase(QWidget * parent)
    : SyModule(parent)
{   
    setModuleName("Database");
    setDescription("Browse profiles listed from the Taxi server.");
    
    SyDatabaseConfig * databaseConfig = new SyDatabaseConfig(0);
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