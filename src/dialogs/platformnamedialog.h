#ifndef PLATFORMNAMEDIALOG_H
#define PLATFORMNAMEDIALOG_H

#include "namedialog.h"

class PlatformNameDialog : public NameDialog
{
	Q_OBJECT

public:
	PlatformNameDialog(QWidget *parent = 0, bool edit = false);

protected:
        virtual int save(QString name, int id = 0, QString fileName = 0);
};

#endif
