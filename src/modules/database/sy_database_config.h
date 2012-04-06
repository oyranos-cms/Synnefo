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


#ifndef SY_DATABASE_CONFIG_H
#define SY_DATABASE_CONFIG_H

#include "symoduleconfig.h"



#include "ui_sy_database_config.h"
#include "ui_sy_database.h"      

class SyDatabaseConfig : public SyModuleConfig, Ui::syDatabaseConfig
{
  Q_OBJECT
  
  public:
    SyDatabaseConfig(QWidget * parent = 0);
    ~SyDatabaseConfig();    
    
    void readSettings();
    
  private:
    Ui::syDatabaseWidget ui_;    // Variable to access 'Settings' widget objects.
    
    void storeWidgets();
};

#endif