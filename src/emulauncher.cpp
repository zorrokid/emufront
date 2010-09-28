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
#include "emulauncher.h"

EmuLauncher::EmuLauncher(QWidget *parent) :
    QWidget(parent)
{
    initWidgets();
    layout();
    //connectSignals();
}

void EmuLauncher::initWidgets()
{
    //micTable = new QTableView(this);
    mediaTypeSelectBox = new QComboBox;
    platformSelectBox = new QComboBox;
    selectButton = new QPushButton(tr("&Update"));
    //populateMediaTypeSelectBox();
    //populatePlatformSelectBox();
}

void EmuLauncher::layout()
{
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(platformSelectBox, 0, 0);
    grid->addWidget(mediaTypeSelectBox, 1, 0);
    grid->addWidget(selectButton, 2, 0);
    //grid->addWidget(micTable, 3, 0);
    setLayout(grid);
}

void EmuLauncher::connectSignals()
{
    connect(selectButton, SIGNAL(clicked()), this, SLOT(updateMediaImageContainers()));
}

void EmuLauncher::populateMediaTypeSelectBox()
{
}

void EmuLauncher::populatePlatformSelectBox()
{
}

void EmuLauncher::updateMediaImageContainers()
{
}
