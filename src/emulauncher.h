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
class Executable;
class MediaImageContainer;
class QProcess;

class EmuLauncher : public QWidget
{
Q_OBJECT
public:
    explicit EmuLauncher(QWidget *parent = 0);
    ~EmuLauncher();
    void updateData();

signals:

private slots:
    void updateMediaImageContainers();
    void launchEmu();
    void processError();

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
    QProcess *proc;
    void initWidgets();
    void layout();
    void connectSignals();
    void populatePlatformSelectBox();
    void populateMediaTypeSelectBox();
    void launch(const Executable*, const MediaImageContainer*);
};

#endif // EMULAUNCHER_H
