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

#ifndef EMULAUNCHER_H
#define EMULAUNCHER_H

#include <QWidget>

class QTableView;
class QPushButton;
class DbMediaType;
class DbPlatform;
class DbMediaImageContainer;
class DbExecutable;
class EFFileObjectComboBox;
class ExecutableComboBox;

class EmuLauncher : public QWidget
{
Q_OBJECT
public:
    explicit EmuLauncher(QWidget *parent = 0);
    void updateData();

signals:

private slots:
    void updateMediaImageContainers();
    void launchEmu();

private:
    QTableView *micTable;
    EFFileObjectComboBox *platformSelectBox;
    EFFileObjectComboBox *mediaTypeSelectBox;
    ExecutableComboBox *execSelectBox;
    QPushButton *selectButton;
    QPushButton *launchButton;
    DbPlatform *dbPlatform;
    DbMediaType *dbMediaType;
    DbMediaImageContainer *dbMic;
    DbExecutable *dbExec;
    void initWidgets();
    void layout();
    void connectSignals();
    void populatePlatformSelectBox();
    void populateMediaTypeSelectBox();
};

#endif // EMULAUNCHER_H
