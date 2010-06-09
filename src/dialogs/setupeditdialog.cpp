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
#include "setupeditdialog.h"
#include "../widgets/stringlistwidget.h"
#include "../db/dbmediatype.h"
#include "../db/dbplatform.h"
#include "../db/dbsetup.h"

SetupEditDialog::SetupEditDialog(QWidget *parent, EmuFrontObject* obj)
    : DataObjectEditDialog(parent, obj)
{
    dbSetup = 0; //new DbSetup(this);
    dbPlatform = 0; //new DbPlatform(this);
    dbMediaType = 0; //new DbMediaType(this);
    initWidgets();
    qDebug() << "Connecting signals";
    connectSignals();
    layout();
}

void SetupEditDialog::initWidgets()
{
    mediaTypeComBox = new QComboBox;
    platformComBox = new QComboBox;
    supportedFileTypesList = new StringListWidget;
    populateMediaTypeComBox();
    populatePlatformComBox();
}

void SetupEditDialog::populateMediaTypeComBox()
{
    qDebug() << "populating media types combo box";
    if (!dbMediaType) dbMediaType = new DbMediaType(this);
    qDebug() << "media type database manager created";
    QSqlQueryModel *model = dbMediaType->getDataModel();
    if (!model) return;
    qDebug() << "We have a media type data model";
    mediaTypeComBox->setModel(model);
    mediaTypeComBox->setModelColumn(DbMediaType::MediaType_Name);
}

void SetupEditDialog::populatePlatformComBox()
{
    qDebug() << "populating platform combo box";
    if (!dbPlatform) dbPlatform = new DbPlatform(this);
    QSqlQueryModel *model = dbPlatform->getDataModel();
    if (!model) return;
    platformComBox->setModel(model);
    platformComBox->setModelColumn(DbPlatform::Platform_Name);
    qDebug() << "platform combo box populated";
}

void SetupEditDialog::layout()
{
    qDebug() << "SetupEditDialog::layout";
   QLabel *platformLabel = new QLabel(tr("&Platform"));
   platformLabel->setBuddy(platformComBox);
   QLabel *mediaTypeLabel = new QLabel(tr("Media&Type"));
   mediaTypeLabel->setBuddy(mediaTypeComBox);
   QGridLayout *gridLayout = new QGridLayout;
   gridLayout->addWidget(platformLabel, 0, 0);
   gridLayout->addWidget(platformComBox, 0, 1);
   gridLayout->addWidget(mediaTypeLabel, 1, 0);
   gridLayout->addWidget(mediaTypeComBox, 1, 1);
   QVBoxLayout *mainLayout = new QVBoxLayout;
   mainLayout->addLayout(gridLayout);
   mainLayout->addWidget(buttonBox);
   setLayout(mainLayout);
    qDebug() << "SetupEditDialog::layout done";

   setWindowTitle(tr("Edit setup"));
}

void SetupEditDialog::acceptChanges()
{
    Setup *sup = dynamic_cast<Setup*>(efObject);
    Platform *plf = getSelectedPlatform();
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
    qDebug() << "Media type selected " << mt->getName();


    Platform *ptmp = sup->getPlatform();
    if (plf != ptmp)
    {
        delete ptmp;
        sup->setPlatform(plf);
    }
    MediaType *mtmp = sup->getMediaType();
    if (mt != mtmp)
    {
        delete mtmp;
        sup->setMediaType(mt);
    }
    sup->setSupportedFileTypeExtensions(supportedFileTypesList->getItems());
    emit dataObjectUpdated();
    efObject = 0;
    close();
}

void SetupEditDialog::setDataObject(EmuFrontObject *ob)
{
    if (!ob) return;
    efObject = ob;
    Setup *sup= dynamic_cast<Setup*>(ob);
    if (sup->getPlatform()) setSelectedPlatform(sup->getPlatform());
    if (sup->getMediaType()) setSelectedMediaType(sup->getMediaType());
    supportedFileTypesList->setItems(sup->getSupportedFileTypeExtensions());
}

void SetupEditDialog::setSelectedPlatform(const Platform *plf)
{
    setSelected(platformComBox, plf, DbPlatform::Platform_Id);
}

void SetupEditDialog::setSelectedMediaType(const MediaType *plf)
{
    setSelected(mediaTypeComBox, plf, DbMediaType::MediaType_Id);
}

Platform* SetupEditDialog::getSelectedPlatform() const
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

MediaType* SetupEditDialog::getSelectedMediaType() const
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
}
