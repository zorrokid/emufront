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
// EmuFront is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.

#include <QtGui>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "mediaimagepathdialog.h"
#include "../db/dbsetup.h"
#include "../dataobjects/filepathobject.h"
#include "../widgets/setupcombobox.h"

MediaImagePathDialog::MediaImagePathDialog(QWidget *parent, EmuFrontObject *efObject)
    : DataObjectEditDialog(parent, efObject, Qt::Horizontal)
{
    initWidgets();
    dbPlatform = 0;
    dbMediaType = 0;
    dbSetup = 0;
    connectSignals();
    layout();
}

MediaImagePathDialog::~MediaImagePathDialog()
{
}

void MediaImagePathDialog::connectSignals()
{
    DataObjectEditDialog::connectSignals();
    connect(filePathButton, SIGNAL(clicked()), this, SLOT(browseFilePath()));
}

void MediaImagePathDialog::browseFilePath()
{
    QString fpath = QFileDialog::getExistingDirectory(this, tr("Select a directory"), ".",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDir d(fpath);
    if (d.exists() && d.isReadable())
    {
        filePathLabel->setText(d.path());
    }
}

void MediaImagePathDialog::initWidgets()
{
    // these widgets will be automatically parented using layout components
    filePathLabel = new QLabel;
    filePathButton = new QPushButton(tr("&Browse filepath"));
    dbSetup = new DbSetup(this);
    setupComBox = new SetupComboBox(dbSetup, this); //QComboBox;
    //populateSetupComBox();
}

/*void MediaImagePathDialog::populateSetupComBox()
{
    dbSetup = new DbSetup(this);
    setupComBox->setModel(dbSetup->getDataModel());
    setupComBox->setModelColumn(DbSetup::Setup_Name);
}*/

void MediaImagePathDialog::layout()
{
   QLabel *setupLabel = new QLabel(tr("&Set up"));
   setupLabel->setBuddy(setupComBox);

   QGridLayout *gridLayout = new QGridLayout;
   gridLayout->addWidget(setupLabel, 0, 0);
   gridLayout->addWidget(setupComBox, 0, 1);
   gridLayout->addWidget(filePathButton, 1, 0);
   gridLayout->addWidget(filePathLabel, 1, 1);
   QVBoxLayout *mainLayout = new QVBoxLayout;
   mainLayout->addLayout(gridLayout);
   mainLayout->addWidget(buttonBox);
   setLayout(mainLayout);

   setWindowTitle(tr("Set media image paths"));
}

void MediaImagePathDialog::setDataObject(EmuFrontObject *ob)
{
    if (!ob) return;
    efObject = ob;
    FilePathObject *fpo = dynamic_cast<FilePathObject*>(ob);
    QString fpath = fpo->getName();
    filePathLabel->setText(fpath);
    if (fpo->getSetup()) setSelectedSetup(fpo->getSetup());
}

void MediaImagePathDialog::setSelectedSetup(const Setup *sup)
{

    setupComBox->setSelected(sup);
    // setSelected(setupComBox, sup, DbSetup::Setup_Id);
}

Setup* MediaImagePathDialog::getSelectedSetup()
{
    EmuFrontObject *ob = setupComBox->getSelected();
    if (!ob) return 0;
    return dynamic_cast<Setup*>(ob);
    /*if (!dbPlatform) dbPlatform => new DbPlatform(this);
    if (!dbMediaType) dbMediaType = new DbMediaType(this);
    Setup *sup = 0;
    int index = setupComBox->currentIndex();
    if (index < 0) return sup;
    QSqlQueryModel *model
        = dynamic_cast<QSqlQueryModel*>(setupComBox->model());
    if (!model)
    {
        errorMessage->showMessage(tr("Data model missing."));
        return sup;
    }
    QSqlRecord rec = model->record(index);
    if (!rec.isEmpty())
    {
        EmuFrontObject *efPlf = dbPlatform->getDataObject(rec.value(DbSetup::Setup_PlatformId).toInt());
        EmuFrontObject *efMt = dbMediaType->getDataObject(rec.value(DbSetup::Setup_MediaTypeId).toInt());

        Platform *plf = dynamic_cast<Platform*>(efPlf);
        MediaType *mt = dynamic_cast<MediaType*>(efMt);
        QString exts = rec.value(DbSetup::Setup_FileTypeExtensions).toString();

        sup = new Setup(rec.value(DbSetup::Setup_Id).toInt(), plf, mt,
            exts.split(DbSetup::FILE_TYPE_EXTENSION_SEPARATOR));
    }
    else errorMessage->showMessage("Record missing");
    return sup;*/
}

void MediaImagePathDialog::acceptChanges()
{
    FilePathObject *fpo = dynamic_cast<FilePathObject*>(efObject);
    Setup *sup = getSelectedSetup();
    if (!sup)
    {
        QMessageBox::information(this, tr("Set up"), tr("Set up not selected"), QMessageBox::Ok);
        return;
    }
    qDebug() << "Setup selected " << sup->getName();
    QString filePath = filePathLabel->text();
    if (filePath.isEmpty())
    {
        QMessageBox::information(this, tr("File path"), tr("File path was not selected"), QMessageBox::Ok);
        return;
    }
    Setup *tmp = fpo->getSetup();
    if (sup != tmp)
    {
        delete tmp;
        fpo->setSetup(sup);
    }
    fpo->setName(filePath);
    emit dataObjectUpdated();
    efObject = 0;
    close();
}
