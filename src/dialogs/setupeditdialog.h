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

#ifndef SETUPEDITDIALOG_H
#define SETUPEDITDIALOG_H

#include "dataobjecteditdialog.h"

//class QComboBox;
class EFFileObjectComboBox;
class DbSetup;
class DbMediaType;
class DbPlatform;
class StringListWidget;
class MediaType;
class Platform;

class SetupEditDialog : public DataObjectEditDialog
{
    Q_OBJECT

public:
    SetupEditDialog(QWidget *parent, EmuFrontObject*);
    //~SetupEditDialog();
    virtual void setDataObject(EmuFrontObject *);
    virtual void updateData();

protected slots:
    virtual void acceptChanges();

private:
    //QComboBox *mediaTypeComBox;
    EFFileObjectComboBox *mediaTypeComBox;
    //QComboBox *platformComBox;
    EFFileObjectComboBox *platformComBox;
    DbSetup *dbSetup;
    DbPlatform *dbPlatform;
    DbMediaType *dbMediaType;
    StringListWidget *supportedFileTypesList;

    void initWidgets();
    void layout();
    void populateMediaTypeComBox();
    void populatePlatformComBox();
    void setSelectedMediaType(const MediaType*);
    void setSelectedPlatform(const Platform*);
    MediaType* getSelectedMediaType() const;
    Platform* getSelectedPlatform() const;
};

#endif // SETUPEDITDIALOG_H
