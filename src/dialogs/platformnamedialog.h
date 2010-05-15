#ifndef PLATFORMNAMEDIALOG_H
#define PLATFORMNAMEDIALOG_H

#include "namedialog.h"

class PlatformNameDialog : public NameDialog
{
	Q_OBJECT

public:
	PlatformNameDialog(QWidget *parent = 0, bool edit = false);

protected:
	virtual void save();
};

#endif
