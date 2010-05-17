#ifndef PLATFORMNAMEDIALOG_H
#define PLATFORMNAMEDIALOG_H

#include "namedialog.h"
#include "../dataobjects/platform.h"

class PlatformNameDialog : public NameDialog
{
	Q_OBJECT

public:
    PlatformNameDialog(QWidget *parent = 0, Platform * = 0);

protected:
    virtual void setDataObject(QString name);
};

#endif
