#ifndef PLATFORMNAMEDIALOG_H
#define PLATFORMNAMEDIALOG_H

#include "namedialog.h"
//#include "../dataobjects/platform.h"

class PlatformNameDialog : public NameDialog
{
	Q_OBJECT

public:
    PlatformNameDialog(QWidget *parent = 0, EmuFrontObject * = 0);

protected:
    virtual int save(QString name, int id = 0, QString fileName = 0);
};

#endif
