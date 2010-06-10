// EmuFront
// Copyright 2010 Mikko Keinänen
//
// This file is part of EmuFront.
//
//
// EmuFront is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Foobar is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#include <QtGui>

#include "../dataobjects/filepathobject.h"
#include "../dataobjects/emufrontfileobject.h"
#include "../db/dbfilepath.h"
#include "../utils/fileutil.h"
#include "mediaimagepathmaindialog.h"
#include "mediaimagepathdialog.h"

MediaImagePathMainDialog::MediaImagePathMainDialog(QWidget *parent)
    : DbObjectDialog(parent)
{
    qDebug() << "MediaImagePathMainDialog";
    setWindowTitle(tr("Set media image paths"));
    qDebug() << "Creating DbFilePath";
    dbManager = new DbFilePath(this);
    qDebug() << "Initializing data table";
    initDataTable();

    scanButton = new QPushButton(tr("&Scan"));
    buttonBox->addButton(scanButton, QDialogButtonBox::ActionRole);

    initEditDialog();
    qDebug() << "Connecting signals";
    // do not move to parent class:
    connectSignals();
}

void MediaImagePathMainDialog::connectSignals()
{
    DbObjectDialog::connectSignals();
    connect(scanButton, SIGNAL(clicked()), this, SLOT(beginScanFilePath()));
}

void MediaImagePathMainDialog::initEditDialog()
{
    qDebug() << "Creating MediaImagePathDialog";
    nameDialog = new MediaImagePathDialog(this, dynamic_cast<FilePathObject*>(dbObject));
}

void MediaImagePathMainDialog::beginScanFilePath()
{
    qDebug() << "Scan file path requested";
    QModelIndex index = objectList->currentIndex();
    FileUtil fileUtil(this);
    if (!index.isValid()) return;
    EmuFrontObject *ob = dbManager->getDataObjectFromModel(&index);
    if (!ob) return;
    FilePathObject *fpo = dynamic_cast<FilePathObject*>(ob);
    try
    {
        QStringList l;
        l << "*.zip"; // TODO set filters in a global constant class

        QList<EmuFrontFileObject*> files = fileUtil.scanFilePath(fpo, l);
    }
    catch (QString s)
    {
        QMessageBox::warning(this, tr("Warning"), s, QMessageBox::Ok);
    }
}

void MediaImagePathMainDialog::scanFilePath(const QString fp, const QStringList filters)
{
    qDebug() << "Will scan file path " << fp;
    QDir dir(fp);
    if (!dir.exists() || !dir.isReadable())
        throw QString(tr("Directory %1 doesn't exists or isn't readable!").arg(fp));

    dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable);

    if (filters.count() > 0) dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        qDebug() << QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.absoluteFilePath());

        QFile f(fileInfo.absoluteFilePath());

    }
}

EmuFrontObject* MediaImagePathMainDialog::createObject()
{
    return new FilePathObject;
}

MediaImagePathMainDialog::~MediaImagePathMainDialog()
{
    deleteCurrentObject();
}

void MediaImagePathMainDialog::deleteCurrentObject()
{
    delete dynamic_cast<FilePathObject*>(dbObject);
    dbObject = 0;
}
