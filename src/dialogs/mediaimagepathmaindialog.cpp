// EmuFront
// Copyright 2010 Mikko Keinänen
//
// This file is part of EmuFront.
//
//
// EmuFront is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation and appearing in the file gpl.txt included in the
// packaging of this file.
//
// EmuFront is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.

#include <QtGui>

#include "filepathobject.h"
#include "emufrontfileobject.h"
#include "dbfilepath.h"
#include "fileutil.h"
#include "mediaimagepathmaindialog.h"
#include "mediaimagepathdialog.h"

MediaImagePathMainDialog::MediaImagePathMainDialog(QWidget *parent)
    : DbObjectDialog(parent)
{
    setWindowTitle(tr("Set media image paths"));
    dbManager = new DbFilePath(this);
    dbMediaImageContainer = new DbMediaImageContainer(this);
    initDataTable();

    scanButton = new QPushButton(tr("&Scan"));
    buttonBox->addButton(scanButton, QDialogButtonBox::ActionRole);

    initEditDialog();
    hiddenColumns << DbFilePath::FilePath_Id;
    hiddenColumns << DbFilePath::FilePath_SetupId;
    hideColumns();

    fileUtil = new FileUtil(this);
    initProgressDialog();

    // do not move to parent class:
    connectSignals();
}

void MediaImagePathMainDialog::initProgressDialog()
{
    progressDialog = new QProgressDialog(this);
    progressDialog->setWindowTitle(tr("Scanning files"));
    progressDialog->setCancelButtonText(tr("Abort"));
    progressDialog->setWindowModality(Qt::WindowModal);
}

void MediaImagePathMainDialog::connectSignals()
{
    DbObjectDialog::connectSignals();
    connect(scanButton, SIGNAL(clicked()), this, SLOT(beginScanFilePath()));
}

void MediaImagePathMainDialog::initEditDialog()
{
    nameDialog = new MediaImagePathDialog(this, dynamic_cast<FilePathObject*>(dbObject));
    connectNameDialogSignals();
}

void MediaImagePathMainDialog::beginScanFilePath()
{
    QModelIndex index = objectList->currentIndex();
    if (!index.isValid()) return;
    if (QMessageBox::question(this,
        tr("Confirm"),
        tr("Do you want to continue? "
        "If you have tons of huge files this may take even hours! "
        "If you are low on battery power, consider carefully!"),
        QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton ) == QMessageBox::No) {
            return;
        }
    FilePathObject *fpo = 0;
    try
    {
        EmuFrontObject *ob = dbManager->getDataObjectFromModel(&index); // throws EmuFrontException
        if (!ob) return;
        fpo = dynamic_cast<FilePathObject*>(ob);
        if (!fpo) return;
        QStringList l;
        l << "*.zip"; // TODO set filters in a global constant class

        dbMediaImageContainer->removeFromFilePath(fpo->getId());

        progressDialog->show();

        setUIEnabled(false);
        int count = fileUtil->scanFilePath(fpo, l, dbMediaImageContainer, progressDialog);
        progressDialog->hide();

        QMessageBox msgBox;
        msgBox.setText(tr("Scanned %1 files to database.").arg(count));
        msgBox.exec();
        DbFilePath *dbfp = dynamic_cast<DbFilePath*>(dbManager);
        if (!(dbfp && dbfp->setScanned(fpo)))
            throw EmuFrontException(tr("Failed updating the last scanned time stamp for selected file path!"));
        else updateList();
    }
    catch (EmuFrontException s)
    {
        errorMessage->showMessage( s.what() );
    }
    setUIEnabled(true);
    delete fpo;
    fpo = 0;
}

EmuFrontObject* MediaImagePathMainDialog::createObject()
{
    return new FilePathObject(FilePathObject::FilePathType_MediaImageDir);
}

MediaImagePathMainDialog::~MediaImagePathMainDialog()
{
    deleteCurrentObject();
}

void MediaImagePathMainDialog::deleteCurrentObject()
{
    if (dbObject) {
        FilePathObject* fpo =  dynamic_cast<FilePathObject*>(dbObject);
        if (fpo) delete fpo;
        else qDebug() << "Failed deleteing FilePathObject";
        dbObject = 0;
    }
}

void MediaImagePathMainDialog::cleanUp()
{
    deleteCurrentObject();
    if (nameDialog) {
        MediaImagePathDialog *pnd =
            dynamic_cast<MediaImagePathDialog*>(nameDialog);
        if (pnd) delete pnd;
        else qDebug() << "Failed to delete MediaImagePathDialog";
        nameDialog = 0;
    }
}
