#ifndef PLATFORMDIALOG_H
#define PLATFORMDIALOG_H

#include "namedialog.h"

class PlatformNameDialog : public NameDialog
{
	Q_OBJECT

public:
	PlatformNameDialog(QWidget *parent = 0, bool edit = false);
};

#endif
