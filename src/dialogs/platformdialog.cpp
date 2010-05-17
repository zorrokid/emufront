#include <QtGui>
#include <QAbstractItemView>
#include <QSqlTableModel>

#include "../dataobjects/platform.h"
#include "platformdialog.h"
#include "platformnamedialog.h"

PlatformDialog::PlatformDialog(QWidget *parent)
    : DbObjectDialog(parent)
{
    setWindowTitle(tr("Set emulated platforms"));
    nameDialog = 0;

    QSqlTableModel *model = dbManager->getPlatforms();
    objectList->setModel(model);
    objectList->setSelectionMode(QAbstractItemView::SingleSelection);
    objectList->setColumnHidden(DatabaseManager::Platform_Id, true);
    objectList->resizeColumnsToContents();
}

int PlatformDialog::deleteObject()
{
    return 0;
}

void PlatformDialog::addObject()
{
    if (!nameDialog)
    {
        if (!dbObject) dbObject = new Platform;
        nameDialog = new PlatformNameDialog(this, static_cast<Platform>(dbObject));
    }
    nameDialog->show();
    nameDialog->raise();
    nameDialog->activateWindow();
}

void PlatformDialog::editObject()
{
}
