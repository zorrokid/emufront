#include <QtGui>
#include <QAbstractItemView>
#include <QSqlTableModel>

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
	nameDialog = new PlatformNameDialog(this);
    }
    nameDialog->show();
    nameDialog->raise();
    nameDialog->activateWindow();
}

void PlatformDialog::editObject()
{
}
