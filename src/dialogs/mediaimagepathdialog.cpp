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
#include <QSqlTableModel>
#include <QSqlRecord>
#include "../db/dbplatform.h"
#include "../db/dbmediatype.h"
#include "../db/dbsetup.h"
#include "mediaimagepathdialog.h"
//#include "../dataobjects/platform.h"
//#include "../dataobjects/mediatype.h"
#include "../dataobjects/filepathobject.h"

MediaImagePathDialog::MediaImagePathDialog(QWidget *parent, EmuFrontObject *efObject)
    : DataObjectEditDialog(parent, efObject, Qt::Horizontal)
{
    qDebug() << "Creating MediaImagePathDialog";
    initWidgets();
    //populateMediaTypeComBox();
    //populatePlatformComBox();
    dbPlatform = 0;
    dbMediaType = 0;
    connectSignals();
    layout();
}

MediaImagePathDialog::~MediaImagePathDialog()
{
    qDebug() << "Destroying MediaImagePathDialog";
}

void MediaImagePathDialog::connectSignals()
{
    DataObjectEditDialog::connectSignals();
    connect(filePathButton, SIGNAL(clicked()), this, SLOT(browseFilePath()));
    qDebug() << "MediaImagePathDialog Connecting signals";
}

void MediaImagePathDialog::browseFilePath()
{
    qDebug() << "Browse file path";
    QString fpath = QFileDialog::getExistingDirectory(this, tr("Select a directory"), ".",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDir d(fpath);
    if (d.exists() && d.isReadable())
    {
        filePathLabel->setText(d.path());
    }
    qDebug() << fpath << " selected.";
}

void MediaImagePathDialog::initWidgets()
{
    qDebug() << "MediaImagePathDialog initializing widgets.";
    // these widgets will be automatically parented using layout components
    filePathLabel = new QLabel;
    filePathButton = new QPushButton(tr("&Browse filepath"));
    /*mediaTypeComBox = new QComboBox;
    platformComBox = new QComboBox;*/
    setupComBox = new QComboBox;
}

void MediaImagePathDialog::populateSetupComBox()
{
    qDebug() << "MediaImagePathDialog populating media types combo box";
    dbSetup = new DbSetup(this);
    setupComBox->setModel(dbSetup->getDataModel());
    setupComBox->setModelColumn(DbSetup::Setup_Name);
}

/*void MediaImagePathDialog::populateMediaTypeComBox()
{
    qDebug() << "MediaImagePathDialog populating media types combo box";
    dbMediaType = new DbMediaType(this);
    mediaTypeComBox->setModel(dbMediaType->getDataModel());
    mediaTypeComBox->setModelColumn(DbMediaType::MediaType_Name);
}

void MediaImagePathDialog::populatePlatformComBox()
{
    qDebug() << "MediaImagePathDialog populating platform combo box";
    dbPlatform = new DbPlatform(this);
    platformComBox->setModel(dbPlatform->getDataModel());
    platformComBox->setModelColumn(DbPlatform::Platform_Name);
}*/

void MediaImagePathDialog::layout()
{
    qDebug() << "MediaImagePathDialog setting layout";
   /*QLabel *platformLabel = new QLabel(tr("&Platform"));
   platformLabel->setBuddy(platformComBox);
   QLabel *mediaTypeLabel = new QLabel(tr("Media&Type"));
   mediaTypeLabel->setBuddy(mediaTypeComBox);*/
   QLabel *setupLabel = new QLabel(tr("&Set up"));
   setupLabel->setBuddy(setupComBox);

   QGridLayout *gridLayout = new QGridLayout;
   /*gridLayout->addWidget(platformLabel, 0, 0);
   gridLayout->addWidget(platformComBox, 0, 1);
   gridLayout->addWidget(mediaTypeLabel, 1, 0);
   gridLayout->addWidget(mediaTypeComBox, 1, 1);*/
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
    /*if (fpo->getPlatform()) setSelectedPlatform(fpo->getPlatform());
    if (fpo->getMediaType()) setSelectedMediaType(fpo->getMediaType());*/
}

void MediaImagePathDialog::setSelectedSetup(const Setup *sup)
{
    setSelected(setupComBox, sup, DbSetup::Setup_Id);
}

/*void MediaImagePathDialog::setSelectedPlatform(const Platform *plf)
{
    setSelected(platformComBox, plf, DbPlatform::Platform_Id);
}

void MediaImagePathDialog::setSelectedMediaType(const MediaType *plf)
{
    setSelected(mediaTypeComBox, plf, DbMediaType::MediaType_Id);
}*/

// TODO: this might be useful to lever to upper classes
void MediaImagePathDialog::setSelected(QComboBox *cbox, const EmuFrontObject *ob, int idIndex)
{
    if (!ob) return;
    QSqlTableModel *model = dynamic_cast<QSqlTableModel*>(cbox->model());
    for (int i = 0; i < model->rowCount(); ++i)
    {
        QSqlRecord rec = model->record(i);
        if (rec.value(idIndex) == ob->getId())
        {
            cbox->setCurrentIndex(i);
            break;
        }
    }
}

Setup* MediaImagePathDialog::getSelectedSetup()
{
    if (!dbPlatform) dbPlatform = new DbPlatform(this);
    if (!dbMediaType) dbMediaType = new DbMediaType(this);
    qDebug() << "MediaImagePathDialog Selecting setup";
    Setup *sup = 0;
    int index = setupComBox->currentIndex();
    qDebug() << "Current index " << index;
    if (index < 0) return sup;
    QSqlQueryModel *model
        = dynamic_cast<QSqlQueryModel*>(setupComBox->model());
    if (!model)
    {
        qDebug() << "Data model missing";
        return sup;
    }
    QSqlRecord rec = model->record(index);
    if (!rec.isEmpty())
    {
        qDebug() << "We have a record";
        EmuFrontObject *efPlf = dbPlatform->getDataObject(rec.value(DbSetup::Setup_PlatformId).toInt());
        EmuFrontObject *efMt = dbMediaType->getDataObject(rec.value(DbSetup::Setup_MediaTypeId).toInt());

        Platform *plf = dynamic_cast<Platform*>(efPlf);
        MediaType *mt = dynamic_cast<MediaType*>(efMt);
        QString exts = rec.value(DbSetup::Setup_FileTypeExtensions).toString();

        sup = new Setup(rec.value(DbSetup::Setup_Id).toInt(), plf, mt,
            exts.split(DbSetup::FILE_TYPE_EXTENSION_SEPARATOR));
    }
    else qDebug() << "Record missing";
    return sup;
}
/*Platform* MediaImagePathDialog::getSelectedPlatform() const
{
    qDebug() << "MediaImagePathDialog Selecting platform";
    Platform *plf = 0;
    int index = platformComBox->currentIndex();
    qDebug() << "Current index " << index;
    if (index < 0) return plf;
    QSqlTableModel* platformModel = dynamic_cast<QSqlTableModel*>(platformComBox->model());
    if (!platformModel)
    {
        qDebug() << "Data model missing";
        return plf;
    }
    QSqlRecord rec = platformModel->record(index);
    if (!rec.isEmpty())
    {
        qDebug() << "We have a record";
        plf = new Platform(rec.value(DbPlatform::Platform_Id).toInt(),
        rec.value(DbPlatform::Platform_Name).toString(),
        rec.value(DbPlatform::Platform_Filename).toString());
    }
    else qDebug() << "Record missing";
    return plf;
}

MediaType* MediaImagePathDialog::getSelectedMediaType() const
{
    MediaType *mt = 0;
    int index = mediaTypeComBox->currentIndex();
    if (index < 0) return mt;
    QSqlTableModel* mediaTypeModel = dynamic_cast<QSqlTableModel*>(mediaTypeComBox->model());
    if (!mediaTypeModel)
    {
        qDebug("Media type data model missing");
        return mt;
    }
    QSqlRecord rec = mediaTypeModel->record(index);
    if (!rec.isEmpty())
        mt = new MediaType(rec.value(DbMediaType::MediaType_Id).toInt(),
                           rec.value(DbMediaType::MediaType_Name).toString(),
                           rec.value(DbMediaType::MediaType_Filename).toString());
    return mt;
}*/

void MediaImagePathDialog::acceptChanges()
{
    qDebug() << "Changes accepted";
    FilePathObject *fpo = dynamic_cast<FilePathObject*>(efObject);
    Setup *sup = getSelectedSetup();
    if (!sup)
    {
        QMessageBox::information(this, tr("Set up"), tr("Set up not selected"), QMessageBox::Ok);
        return;
    }
    qDebug() << "Setup selected " << sup->getName();
    /*Platform *plf = getSelectedPlatform();
    if (!plf)
    {
        QMessageBox::information(this, tr("Platform"), tr("Platform not selected"), QMessageBox::Ok);
        return;
    }
    qDebug() << "Platform selected " << plf->getName();
    MediaType *mt = getSelectedMediaType();
    if (!mt)
    {
        QMessageBox::information(this, tr("Media type"), tr("Media type was not selected"), QMessageBox::Ok);
        return;
    }
    qDebug() << "Media type selected " << mt->getName();*/
    QString filePath = filePathLabel->text();
    if (filePath.isEmpty())
    {
        QMessageBox::information(this, tr("File path"), tr("File path was not selected"), QMessageBox::Ok);
        return;
    }
    qDebug() << "File path selected " << filePath;
    Setup *tmp = fpo->getSetup();
    if (sup != tmp)
    {
        delete tmp;
        fpo->setSetup(sup);
    }
    /*Platform *ptmp = fpo->getPlatform();
    if (plf != ptmp)
    {
        delete ptmp;
        fpo->setPlatform(plf);
    }
    MediaType *mtmp = fpo->getMediaType();
    if (mt != mtmp)
    {
        delete mtmp;
        fpo->setMediaType(mt);
    }*/
    fpo->setName(filePath);
    emit dataObjectUpdated();
    efObject = 0;
    close();
}

void MediaImagePathDialog::rejectChanges()
{
    // we don't delete the data object here
    efObject = 0;
    emit updateRejected();
    close();
}
