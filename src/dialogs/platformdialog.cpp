#include <QtGui>
#include <QAbstractItemView>
#include <QSqlTableModel>
#include <QTextStream>

#include "../dataobjects/platform.h"
#include "platformdialog.h"
#include "platformnamedialog.h"


QTextStream cout(stdout, QIODevice::WriteOnly);

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
    cout << "PlaformDialog::addObject" << endl;
    if (!nameDialog)
    {
        if (!dbObject) dbObject = new Platform;
        cout << "PlaformDialog::addObject: creating nameDialog..." << endl;
        nameDialog = new PlatformNameDialog(this, dynamic_cast<Platform*>(dbObject));
    }
    nameDialog->show();
    nameDialog->raise();
    nameDialog->activateWindow();
}

void PlatformDialog::editObject()
{
}
