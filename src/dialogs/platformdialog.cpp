#include "platformdialog.h"

PlatformDialog::PlatformDialog(QWidget *parent)
    : DbObjectDialog(parent)
{
    setWindowTitle(tr("Set emulated platforms"));
}
