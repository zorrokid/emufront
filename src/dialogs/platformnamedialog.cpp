#include "platformnamedialog.h"

PlatformNameDialog::PlatformNameDialog(QWidget *parent, bool edit)
	: NameDialog(parent, edit)
{
	setWindowTitle(tr("Set platform name"));
}
