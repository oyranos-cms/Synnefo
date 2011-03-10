#ifndef SYNNEFO_H
#define SYNNEFO_H

#include <QApplication>
#include <QSignalMapper>
#include <QWidget>

// Qt Designer code translation.
#include "ui_synnefo-main.h"          

class Synnefo : public QMainWindow
{
    Q_OBJECT
    
  public:
    Synnefo (QWidget * parent);
    ~Synnefo();
    
  private:
    Ui::syMainWindow syInterface;            
};

#endif