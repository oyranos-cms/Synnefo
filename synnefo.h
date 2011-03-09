#ifndef SYNNEFO_H
#define SYNNEFO_H

#include <QApplication>
#include <QSignalMapper>
#include <QWidget>

#include "ui_syfoMain.h"

class Synnefo : public QMainWindow
{
    Q_OBJECT
    
  public:
    Synnefo (QWidget * parent = 0);
    
  private:
    Ui::Synnefo_Main syfo_ui;
    
};

#endif