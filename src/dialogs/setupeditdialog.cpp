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
#include <QSqlTableModel>
#include <QSqlRecord>
#include "setupeditdialog.h"
#include "../widgets/stringlistwidget.h"
#include "../widgets/effileobjectcombobox.h"
#include "../db/dbmediatype.h"
#include "../db/dbplatform.h"
#include "../db/dbsetup.h"

SetupEditDialog::SetupEditDialog(QWidget *parent, EmuFrontObject* obj)
    : DataObjectEditDialog(parent, obj)
{
    dbSetup = 0;
    dbPlatform = new DbPlatform(this);
    dbMediaType = new DbMediaType(this);
    initWidgets();
    connectSignals();
    layout();
}

void SetupEditDialog::initWidgets()
{
    mediaTypeComBox = new EFFileObjectComboBox(dbMediaType, this);
    platformComBox = new EFFileObjectComboBox(dbPlatform, this);
    supportedFileTypesList = new StringListWidget;
}

void SetupEditDialog::layout()
{
   QLabel *platformLabel = new QLabel(tr("&Platform"));
   platformLabel->setBuddy(platformComBox);
   QLabel *mediaTypeLabel = new QLabel(tr("Media&Type"));
   mediaTypeLabel->setBuddy(mediaTypeComBox);
   QGridLayout *gridLayout = new QGridLayout;
   gridLayout->addWidget(platformLabel, 0, 0);
   gridLayout->addWidget(platformComBox, 0, 1);
   gridLayout->addWidget(mediaTypeLabel, 1, 0);
   gridLayout->addWidget(mediaTypeComBox, 1, 1);
   gridLayout->addWidget(supportedFileTypesList, 2, 0, 2, 2);
   QVBoxLayout *mainLayout = new QVBoxLayout;
   mainLayout->addLayout(gridLayout);
   mainLayout->addWidget(buttonBox);
   setLayout(mainLayout);
   setWindowTitle(tr("Edit setup"));
}

void SetupEditDialog::acceptChanges()
{
    Setup *sup = dynamic_cast<Setup*>(efObject);
    Platform *plf = getSelectedPlatform();
    if (!plf)
    {
        QMessageBox::information(this, tr("Platform"),
            tr("Platform not selected"), QMessageBox::Ok);
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
    qDebug() << "Closing setup edit dialog";
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
    platformComBox->setSelected(plf);
}

void SetupEditDialog::setSelectedMediaType(const MediaType *plf)
{
    mediaTypeComBox->setSelected(plf);
}

Platform* SetupEditDialog::getSelectedPlatform() const
{
    EmuFrontObject *o = platformComBox->getSelected();
    if (!o) return 0;
    Platform *plf = dynamic_cast<Platform*>(o);
    return plf;
}

MediaType* SetupEditDialog::getSelectedMediaType() const
{
    EmuFrontObject *o = mediaTypeComBox->getSelected();
    if (!o) return 0;
    MediaType *mt = dynamic_cast<MediaType*>(o);
    return mt;
}

void SetupEditDialog::updateData()
{
    platformComBox->updateDataModel();
    mediaTypeComBox->updateDataModel();
}
