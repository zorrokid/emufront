/*
** EmuFront
** Copyright 2010-2011 Mikko Keinänen
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
#include <QtGui>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "setupeditdialog.h"
#include "fileextensionwidget.h"
#include "effileobjectcombobox.h"
#include "dbmediatype.h"
#include "dbplatform.h"
#include "dbsetup.h"

SetupEditDialog::SetupEditDialog(QWidget *parent, EmuFrontObject* obj)
    : DataObjectEditDialog(parent, obj)
{
    dbSetup = 0;
    dbPlatform = new DbPlatform(this);
    dbMediaType = new DbMediaType(this);
    initWidgets();
    connectSignals();
    emit test();
    layout();
}

void SetupEditDialog::initWidgets()
{
    mediaTypeComBox = new EFFileObjectComboBox(dbMediaType, this);
    platformComBox = new EFFileObjectComboBox(dbPlatform, this);
    supportedFileTypesList = new FileExtensionWidget;
    supportedFileTypesList->setToolTip(tr("Set the supported file types by entering the file extension(s) (no wildcards or dots!)."));
}

void SetupEditDialog::layout()
{
   QLabel *platformLabel = new QLabel(tr("&Platform:"));
   platformLabel->setBuddy(platformComBox);
   QLabel *mediaTypeLabel = new QLabel(tr("Media&Type:"));
   mediaTypeLabel->setBuddy(mediaTypeComBox);
   QLabel *fileTypesLabel = new QLabel(tr("Supported file types:"));
   QGridLayout *gridLayout = new QGridLayout;
   gridLayout->addWidget(platformLabel, 0, 0);
   gridLayout->addWidget(platformComBox, 0, 1);
   gridLayout->addWidget(mediaTypeLabel, 1, 0);
   gridLayout->addWidget(mediaTypeComBox, 1, 1);
   gridLayout->addWidget(fileTypesLabel, 2, 0, 1, 2);
   gridLayout->addWidget(supportedFileTypesList, 3, 0, 2, 2);
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


    bool change = false;
    Platform *ptmp = sup->getPlatform();
    if (!ptmp || *plf != *ptmp)
    {
        delete ptmp;
        sup->setPlatform(plf);
        change = true;
    }
    MediaType *mtmp = sup->getMediaType();
    if (!mtmp || *mt != *mtmp)
    {
        delete mtmp;
        sup->setMediaType(mt);
        change = true;
    }

    // "Two lists are considered equal if they contain the same values in the same order."
    if (supportedFileTypesList->getItems() != sup->getSupportedFileTypeExtensions()) {
        sup->setSupportedFileTypeExtensions(supportedFileTypesList->getItems());
        change = true;
    }
    // TODO: the second time around this signal is not received!
    if (change) emit dataObjectUpdated();
    efObject = 0;
    close();
}

void SetupEditDialog::setDataObject(EmuFrontObject *ob)
{
    if (!ob) return;
    qDebug() << "Updating Setup edit dialog data object to "
        << ob->getName() << ".";
    if (efObject)
        delete dynamic_cast<Setup*>(efObject); // TODO: caused crash if another instance of setupeditdialog was created and new instance destroyed object being referenced in another existing dialog.
    efObject = ob;
    Setup *sup= dynamic_cast<Setup*>(ob);
    if (!sup) {
        qDebug() << "Failed casting to Setup";
        return;
    }
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

/* Returns a pointer to an object which must be deleted by calling code */
Platform* SetupEditDialog::getSelectedPlatform()
{
    EmuFrontObject *o = 0;
    try { o = platformComBox->getSelected();  }
    catch(EmuFrontException &e){
        errorMessage->showMessage(e.what());
    }

    if (!o) return 0;
    Platform *plf = dynamic_cast<Platform*>(o);
    return plf;
}

/* Returns a pointer to an object which must be deleted by calling code */
MediaType* SetupEditDialog::getSelectedMediaType()
{
    EmuFrontObject *o = 0;
    try { o = mediaTypeComBox->getSelected(); }
    catch(EmuFrontException &e){ errorMessage->showMessage(e.what()); }
    if (!o) return 0;
    MediaType *mt = dynamic_cast<MediaType*>(o);
    return mt;
}

void SetupEditDialog::updateData()
{
    platformComBox->updateDataModel();
    mediaTypeComBox->updateDataModel();
}

void SetupEditDialog::clear()
{
    platformComBox->setCurrentIndex(-1);
    mediaTypeComBox->setCurrentIndex(-1);
    supportedFileTypesList->clear();
}
