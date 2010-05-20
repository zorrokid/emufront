#include "platformnamedialog.h"
#include "../db/databasemanager.h"

PlatformNameDialog::PlatformNameDialog(QWidget *parent, Platform *efObj)
    : NameDialog(parent, efObj)
{
    setWindowTitle(tr("Set platform name"));
}

void PlatformNameDialog::setDataObject(QString name)
{
    efObject->setName(name);
    (dynamic_cast<Platform*>(efObject))->setFilename("");
}

void PlatformNameDialog::setDataObject(EmuFrontObject *ob)
{

    efObject = dynamic_cast<Platform*>(ob);
}
