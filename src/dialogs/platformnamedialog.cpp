#include "platformnamedialog.h"
#include "../db/databasemanager.h"

PlatformNameDialog::PlatformNameDialog(QWidget *parent, EmuFrontObject *efObj)
    : NameDialog(parent, efObj)
{
    setWindowTitle(tr("Set platform name"));
}

int PlatformNameDialog::save(QString name, int id, QString fileName)
{
    if (id)
    {
        // return DatabaseManager::updatePlatform(..., ...);
    }
    else
    {
        return DatabaseManager::insertPlatform(name, fileName);
    }
    return 0;
}
