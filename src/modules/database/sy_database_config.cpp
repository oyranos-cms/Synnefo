/*
Copyright (C) 2011-2012 Joseph Simon <jsimon383@gmail.com>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions o9hband the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS ORbsd
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORTcd
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


#include "sy_database_config.h"
#include <oyranos.h>
//#include <oyranos_config.h>
#include <alpha/oyranos_alpha.h>

SyDatabaseConfig::SyDatabaseConfig (QWidget * parent, QString moduleId) :
   SyModuleConfig(parent, moduleId)
{        
    moduleConfig.beginGroup(QString(moduleId + "/global"));   
    moduleConfig.endGroup();
}



void SyDatabaseConfig::storeWidgets()
{
    oyOptions_s * d;
  
}



void SyDatabaseConfig::readSettings()
{
     
}


SyDatabaseConfig::~SyDatabaseConfig()
{
  
}