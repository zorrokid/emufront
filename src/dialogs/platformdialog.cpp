#include "platformdialog.h"

PlatformDialog::PlatformDialog(QWidget *parent)
    : DbObjectDialog(parent)
{
    setWindowTitle(tr("Set emulated platforms"));
}

int PlatformDialog::deleteObject()
{
    return 0;
}

void PlatformDialog::addObject()
{
}

void PlatformDialog::editObject()
{
}
