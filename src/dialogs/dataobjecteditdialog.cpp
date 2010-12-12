/*
** EmuFront
** Copyright 2010 Mikko Keinänen
**
** This file is part of EmuFront.
**
**
** EmuFront is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License version 2 as published by
** the Free Software Foundation and appearing in the file gpl.txt included in the
** packaging of this file.
**
** EmuFront is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QtGui>
#include <QDialogButtonBox>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QComboBox>
#include <QDebug>
#include "dataobjecteditdialog.h"

DataObjectEditDialog::DataObjectEditDialog(QWidget *parent, EmuFrontObject *ob, Qt::Orientation orientation)
    : EmuFrontDialog(parent), efObject(ob), orientation(orientation)
{
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Abort, orientation);
}

void DataObjectEditDialog::connectSignals()
{
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptChanges()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(rejectChanges()));
}

void DataObjectEditDialog::closeEvent(QCloseEvent *ev)
{
    qDebug() << "Emitting dialog closed";
    emit dialogClosed();
    ev->accept();
}


void DataObjectEditDialog::rejectChanges()
{
    efObject = 0;
    emit updateRejected();
    close();
}

void DataObjectEditDialog::updateData()
{
}
