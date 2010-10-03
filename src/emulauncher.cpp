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
#include "emulauncher.h"
#include "db/dbmediatype.h"
#include "db/dbplatform.h"
#include "db/dbmediaimagecontainer.h"
#include "widgets/effileobjectcombobox.h"

EmuLauncher::EmuLauncher(QWidget *parent) :
    QWidget(parent)
{
    dbPlatform = new DbPlatform(this);
    dbMediaType = new DbMediaType(this);
    dbMic = 0;
    initWidgets();
    layout();
    connectSignals();
}

void EmuLauncher::updateData()
{
    platformSelectBox->updateDataModel();
    mediaTypeSelectBox->updateDataModel();
}

void EmuLauncher::initWidgets()
{
    micTable = new QTableView(this);
    micTable->setSelectionMode(QAbstractItemView::SingleSelection);
    mediaTypeSelectBox = new EFFileObjectComboBox(dbMediaType, this);
    platformSelectBox = new EFFileObjectComboBox(dbPlatform, this);
    selectButton = new QPushButton(tr("&Update"));
}

void EmuLauncher::layout()
{
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(platformSelectBox, 0, 0);
    grid->addWidget(mediaTypeSelectBox, 1, 0);
    grid->addWidget(selectButton, 1, 1);
    grid->addWidget(micTable, 2, 0, 1, 2);
    setLayout(grid);
}

void EmuLauncher::connectSignals()
{
    connect(selectButton, SIGNAL(clicked()), this, SLOT(updateMediaImageContainers()));
}

void EmuLauncher::updateMediaImageContainers()
{
    qDebug() << "updateMediaImageContainers slot";
    int mtid = mediaTypeSelectBox->getSelected()
        ? mediaTypeSelectBox->getSelected()->getId()
        : -1;
    int plfid = platformSelectBox->getSelected()
        ? platformSelectBox->getSelected()->getId()
        : -1;

    if (!dbMic) dbMic = new DbMediaImageContainer(this);
    dbMic->filter(mtid, plfid);
    micTable->setModel(dbMic->getDataModel());
    micTable->resizeColumnsToContents();
    platformSelectBox->updateDataModel();
    mediaTypeSelectBox->updateDataModel();
}

