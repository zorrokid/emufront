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

#ifndef EXECUTABLEEDITDIALOG_H
#define EXECUTABLEEDITDIALOG_H

#include "dataobjecteditdialog.h"

class DbExecutable;
class DbSetup;
class Setup;
class SetupComboBox;
class QLineEdit;

class ExecutableEditDialog : public DataObjectEditDialog
{
    Q_OBJECT
public:
    ExecutableEditDialog(QWidget *parent, EmuFrontObject*);
    virtual void setDataObject(EmuFrontObject *);
    virtual void updateData();

protected slots:
    virtual void acceptChanges();

protected:
    virtual void clear();

private:
    DbExecutable *dbExecutable;
    DbSetup *dbSetup;
    SetupComboBox *setupComBox;
    void initWidgets();
    void layout();
    void setSelectedSetup(const Setup*);
    Setup* getSelectedSetup();
    QLineEdit *nameEdit;
    QLineEdit *execEdit;
    QLineEdit *optEdit;

};

#endif // EXECUTABLEEDITDIALOG_H
