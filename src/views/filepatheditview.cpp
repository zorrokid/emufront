/*
** EmuFront
** Copyright 2010 Mikko Keinänen
**
** This file is part of EmuFront.
**
**
** EmuFront is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License version 2 as published by
** the Free Software Foundation and appearing in the file gpl.txt included in the
** packaging of this file.
**
** EmuFront is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "filepatheditview.h"
#include "filepathmodel.h"
#include "fileutil.h"
#include "setupmodel.h"
#include "comboboxdelegate.h"
#include "filesystembrowsedelegate.h"
#include <QtGui>

FilePathEditView::FilePathEditView(SetupModel *supModel, QWidget *parent) :
    EmuFrontEditView(parent)
{
    setWindowTitle(tr("Set media image paths"));
    scanButton = new QPushButton(tr("&Scan"));
    buttonBox->addButton(scanButton, QDialogButtonBox::ActionRole);
    fileUtil = new FileUtil(this);
    initProgressDialog();

    model = new FilePathModel(this);
    objectList->setModel(model);
    ComboBoxDelegate *setupDelegate = new ComboBoxDelegate(
        supModel,
        SetupModel::Setup_Id,
        SetupModel::Setup_Name,
        this
    );
    objectList->setItemDelegateForColumn(FilePathModel::FilePath_SetupId, setupDelegate);
    FileSystemBrowseDelegate *fsBrowseDelegate = new FileSystemBrowseDelegate(this);
    objectList->setItemDelegateForColumn(FilePathModel::FilePath_Name, fsBrowseDelegate);
    postInit();
}

void FilePathEditView::initProgressDialog()
{
    progressDialog = new QProgressDialog(this);
    progressDialog->setWindowTitle(tr("Scanning files"));
    progressDialog->setCancelButtonText(tr("Abort"));
    progressDialog->setWindowModality(Qt::WindowModal);
}

void FilePathEditView::connectSignals()
{
    EmuFrontEditView::connectSignals();
    connect(scanButton, SIGNAL(clicked()), this, SLOT(beginScanFilePath()));
}

void FilePathEditView::beginScanFilePath()
{
    QModelIndex index = objectList->currentIndex();
    if (!index.isValid()) return;

    if (QMessageBox::question(this,
        tr("Confirm"),
        tr("Do you want to continue? "
        "Re-scanning file path removes old entries for selected path. "
        "If you have tons of huge files this may take even hours! "
        "If you are low on battery power, consider carefully!"),
        QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton ) == QMessageBox::No)
    { return; }

    FilePathModel *fpModel = qobject_cast<FilePathModel*>(model);
    FilePathObject *fpo = fpModel->getFilePathObject(index);
    if (!fpo) {
        errorMessage->showMessage(tr("Failed creating a file path object of selected file path."));
        return;
    }
    try
    {
        QStringList l;
        l << "*.zip"; // TODO set filters in a global constant class

        progressDialog->show();
        //setUIEnabled(false);
        int count = fileUtil->scanFilePath(fpo, l, progressDialog);
        progressDialog->hide();

        QMessageBox msgBox;
        msgBox.setText(tr("Scanned %1 files to database.").arg(count)); msgBox.exec();
        fpModel->setScanned(fpo->getId());
        //updateList();
    }
    catch (EmuFrontException s)
    {
        errorMessage->showMessage( s.what() );
    }
    //setUIEnabled(true);
    delete fpo;
    fpo = 0;
}

void FilePathEditView::setHiddenColumns()
{
	hiddenColumns << FilePathModel::FilePath_FileTypeId;
	hiddenColumns << FilePathModel::FilePath_Id;
	hiddenColumns << FilePathModel::FilePath_SetupName;
}
