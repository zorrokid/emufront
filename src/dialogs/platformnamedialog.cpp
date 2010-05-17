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
    efObject->setFilename("");
    /*if (id)
    {
        // return DatabaseManager::updatePlatform(..., ...);
    }
    else
    {
        return DatabaseManager::insertPlatform(name, fileName);
    }
    return 0;*/
}
