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
#include "../db/dbplatform.h"
#include "../db/dbmediatype.h"
#include "mediaimagepathdialog.h"

MediaImagePathDialog::MediaImagePathDialog(QWidget *parent, EmuFrontObject *efObject)
    : DataObjectEditDialog(parent, efObject)
{
    initWidgets();
    populateMediaTypeComBox();
    populatePlatformComBox();
    layout();
    connectSignals();
}

void MediaImagePathDialog::connectSignals()
{
}

void MediaImagePathDialog::initWidgets()
{
    // these widgets will be automatically parented using layout components
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
    filePathLabel = new QLabel;
    filePathButton = new QPushButton(tr("&Browse filepath"));
    mediaTypeComBox = new QComboBox;
    platformComBox = new QComboBox;
}

void MediaImagePathDialog::populateMediaTypeComBox()
{
}

void MediaImagePathDialog::populatePlatformComBox()
{
}

void MediaImagePathDialog::layout()
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
   gridLayout->addWidget(filePathButton, 2, 0);
   gridLayout->addWidget(filePathLabel, 2, 1);
   gridLayout->addWidget(buttonBox, 3, 0, 1, 2);
   setLayout(gridLayout);

   setWindowTitle(tr("Set media image paths"));
}

void MediaImagePathDialog::setDataObject(EmuFrontObject *)
{
}
