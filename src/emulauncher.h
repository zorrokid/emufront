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
#include <QProcess>

class QTableView;
class QPushButton;
class QErrorMessage;
class DbMediaType;
class DbPlatform;
class DbMediaImageContainer;
class DbExecutable;
class EFFileObjectComboBox;
class ExecutableComboBox;
class Executable;
class MediaImageContainer;
class EmuHelper;

class EmuLauncher : public QWidget
{
Q_OBJECT
public:
    EmuLauncher(QErrorMessage *, QWidget *parent = 0, QString tmp = ".");
    ~EmuLauncher();
    void updateData();
    void setTmpDirPath(QString);

signals:

private slots:
    void updateMediaImageContainers();
    void launchEmu();
    void processError(QProcess::ProcessError);
    void processFinished(int);

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
    EmuHelper *emuHelper;
    void initWidgets();
    void layout();
    void connectSignals();
    void populatePlatformSelectBox();
    void populateMediaTypeSelectBox();
    void launch(const Executable*, const MediaImageContainer*);
    void cleanTmp();
    QString tmpDirPath;
    QErrorMessage *errorMessage;
};

#endif // EMULAUNCHER_H
