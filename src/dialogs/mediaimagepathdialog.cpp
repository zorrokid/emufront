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
#include "mediaimagepathdialog.h"
#include "../dataobjects/platform.h"
#include "../dataobjects/mediatype.h"
#include "../dataobjects/filepathobject.h"

MediaImagePathDialog::MediaImagePathDialog(QWidget *parent, EmuFrontObject *efObject)
    : DataObjectEditDialog(parent, efObject)
{
    qDebug() << "Creating MediaImagePathDialog";
    initWidgets();
    populateMediaTypeComBox();
    populatePlatformComBox();
    layout();
    connectSignals();
}

MediaImagePathDialog::~MediaImagePathDialog()
{
    qDebug() << "Destroying MediaImagePathDialog";
}

void MediaImagePathDialog::connectSignals()
{
    qDebug() << "MediaImagePathDialog Connecting signals";
}

void MediaImagePathDialog::initWidgets()
{
    qDebug() << "MediaImagePathDialog initializing widgets.";
    // these widgets will be automatically parented using layout components
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
    filePathLabel = new QLabel;
    filePathButton = new QPushButton(tr("&Browse filepath"));
    mediaTypeComBox = new QComboBox;
    platformComBox = new QComboBox;
}

void MediaImagePathDialog::populateMediaTypeComBox()
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
}

void MediaImagePathDialog::layout()
{
    qDebug() << "MediaImagePathDialog setting layout";
   QLabel *platformLabel = new QLabel(tr("&Platform"));
   platformLabel->setBuddy(platformComBox);
   QLabel *mediaTypeLabel = new QLabel(tr("Media&Type"));
   mediaTypeLabel->setBuddy(mediaTypeComBox);

   QGridLayout *gridLayout = new QGridLayout;
   gridLayout->addWidget(platformLabel, 0, 0);
   gridLayout->addWidget(platformComBox, 0, 1);
   gridLayout->addWidget(mediaTypeLabel, 1, 0);
   gridLayout->addWidget(mediaTypeComBox, 1, 1);
   gridLayout->addWidget(filePathButton, 2, 0);
   gridLayout->addWidget(filePathLabel, 2, 1);
   gridLayout->addWidget(buttonBox, 3, 0, 1, 2);
   setLayout(gridLayout);

   setWindowTitle(tr("Set media image paths"));
}

void MediaImagePathDialog::setDataObject(EmuFrontObject *ob)
{
    if (!ob) return;
    efObject = ob;
    FilePathObject *fpo = dynamic_cast<FilePathObject*>(ob);
    QString fpath = fpo->getName();
    filePathLabel->setText(fpath);
    if (fpo->getPlatform()) setSelectedPlatform(fpo->getPlatform());
    if (fpo->getMediaType()) setSelectedMediaType(fpo->getMediaType());
}

void MediaImagePathDialog::setSelectedPlatform(const Platform *plf)
{
    setSelected(platformModel, platformComBox, plf, DbPlatform::Platform_Id);
}

void MediaImagePathDialog::setSelectedMediaType(const MediaType *plf)
{
    setSelected(mediaTypeModel, mediaTypeComBox, plf, DbMediaType::MediaType_Id);
}

// TODO: this might be useful to lever to upper classes
void MediaImagePathDialog::setSelected(const QSqlTableModel *model, QComboBox *cbox, const EmuFrontObject *ob, int idIndex)
{
    if (!ob) return;
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


void MediaImagePathDialog::acceptChanges()
{
}

void MediaImagePathDialog::rejectChanges()
{
    // we don't
    efObject = 0;
}
