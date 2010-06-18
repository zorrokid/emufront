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

#ifndef NAMEDIALOG_H
#define NAMEDIALOG_H

#include "../dataobjects/emufrontobject.h"
#include "dataobjecteditdialog.h"

class QLabel;
class QLineEdit;
class QPushButton;

class NameDialog : public DataObjectEditDialog
{
	Q_OBJECT

public:
    NameDialog(QWidget *parent = 0, EmuFrontObject * = 0);
    ~NameDialog();
    virtual void setDataObject(EmuFrontObject *) = 0;

signals:
    void test();

protected slots:
    virtual void acceptChanges();
    void enableSaveButton(const QString &);

protected:
    virtual void setDataObject(QString name) =0;
    QLabel *nameLabel;
	QLineEdit *nameEdit;

private:
	void connectSignals();
	void layout();
};

#endif
