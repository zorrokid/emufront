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

#ifndef MEDIAIMAGEPATHDIALOG_H
#define MEDIAIMAGEPATHDIALOG_H

#include "dataobjecteditdialog.h"

class QDialogButtonBox;
class QComboBox;
class QLabel;
class QPushButton;
class QSqlTableModel;
class DbSetup;
class Setup;
class DbMediaType;
class DbPlatform;

class MediaImagePathDialog : public DataObjectEditDialog
{
    Q_OBJECT

public:
    MediaImagePathDialog(QWidget *parent, EmuFrontObject*);
    ~MediaImagePathDialog();
    virtual void setDataObject(EmuFrontObject *);

protected slots:
    virtual void acceptChanges();
    void browseFilePath();

private:
    QComboBox *setupComBox;
    QLabel *filePathLabel;
    QPushButton *filePathButton;
    DbSetup *dbSetup;
    DbMediaType *dbMediaType;
    DbPlatform *dbPlatform;

    void initWidgets();
    void layout();
    void connectSignals();
    void populateSetupComBox();
    void setSelectedSetup(const Setup *);
    Setup* getSelectedSetup();
};

#endif // MEDIAIMAGEPATHDIALOG_H
