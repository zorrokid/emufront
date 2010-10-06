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
#include "executableeditdialog.h"
#include "../db/dbexecutable.h"
#include "../db/dbsetup.h"
#include "../dataobjects/executable.h"
#include "../dataobjects/setup.h"
#include "../widgets/setupcombobox.h"
#include "../exceptions/emufrontexception.h"

ExecutableEditDialog::ExecutableEditDialog(QWidget *parent, EmuFrontObject *obj)
    : DataObjectEditDialog(parent, obj)
{
    dbExecutable = 0;
    dbSetup = new DbSetup(this);
    initWidgets();
    connectSignals();
    layout();
}

void ExecutableEditDialog::initWidgets()
{
    setupComBox = new SetupComboBox(dbSetup, this);
    nameEdit = new QLineEdit;
    nameEdit->setToolTip(tr("Set an individual short "
        "description for this emulator configuration."));
    execEdit = new QLineEdit;
    execEdit->setToolTip(tr("Emulator executable in $PATH "
        "or absolute path to emulator executable."));
    optEdit = new QLineEdit;
    optEdit->setToolTip(tr("Command line parameters for the "
        "emulator executable including $1 as file 1 to be "
        "assigned to emulator, $2 as file 2, etc."));
}

void ExecutableEditDialog::layout()
{
    QLabel *nameLabel = new QLabel(tr("&Description"));
    nameLabel->setBuddy(nameEdit);
    QLabel *execLabel = new QLabel(tr("&Executable"));
    execLabel->setBuddy(execEdit);
    QLabel *optLabel = new QLabel(tr("&Parameters"));
    optLabel->setBuddy(optEdit);
    QLabel *setupLabel = new QLabel(tr("&Setup"));
    setupLabel->setBuddy(setupComBox);
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(nameLabel, 0, 0);
    gridLayout->addWidget(nameEdit, 0, 1);
    gridLayout->addWidget(execLabel, 1, 0);
    gridLayout->addWidget(execEdit, 1, 1);
    gridLayout->addWidget(optLabel, 2, 0);
    gridLayout->addWidget(optEdit, 2, 1);
    gridLayout->addWidget(setupLabel, 3, 0);
    gridLayout->addWidget(setupComBox, 3, 1);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    setWindowTitle(tr("Edit emulator configuration"));
}

void ExecutableEditDialog::acceptChanges()
{
    Executable *ex = dynamic_cast<Executable*>(efObject);
    try {
        Setup *su = getSelectedSetup();
        if (!su) {
            throw new EmuFrontException(tr("Setup not selected"));
        }
        QString name = nameEdit->text().trimmed();
        if (name.isEmpty()){
            throw new EmuFrontException(tr("Name is not set"));
        }
        QString exec = execEdit->text().trimmed();
        if (exec.isEmpty()){
            throw new EmuFrontException(tr("Executable is not set"));
        }
        QString opts = optEdit->text().trimmed();
        if (opts.isEmpty()) {
            throw new EmuFrontException(tr("Options not set"));
        }
        Setup *supTmp = ex->getSetup();
        if (supTmp != su) {
            delete supTmp;
            ex->setSetup(su);
        }
        ex->setName(name);
        ex->setExecutable(exec);
        ex->setOptions(opts);
        emit dataObjectUpdated();
        efObject = 0;
        close();
    } catch(EmuFrontException x) {
            QMessageBox::information(this,
                                     tr("Updating emulator"),
                                     x.what(), QMessageBox::Ok);
            return;
    }
}

void ExecutableEditDialog::setDataObject(EmuFrontObject *ob)
{
    if (!ob) return;
    efObject = ob;
    Executable *ex = dynamic_cast<Executable*>(ob);
    if (ex->getSetup() && ex->getSetup()->getId() >= 0)
        setSelectedSetup(ex->getSetup());
    nameEdit->setText(ex->getName());
    execEdit->setText(ex->getExecutable());
    optEdit->setText(ex->getOptions());
}

void ExecutableEditDialog::setSelectedSetup(const Setup *su)
{
    setupComBox->setSelected(su);
}

Setup* ExecutableEditDialog::getSelectedSetup() const
{
    EmuFrontObject *o = setupComBox->getSelected();
    if (!o) return 0;
    Setup *ex = dynamic_cast<Setup*>(o);
    return ex;
}

void ExecutableEditDialog::updateData()
{
    setupComBox->updateDataModel();
}
