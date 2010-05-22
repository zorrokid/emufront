// EmuFront
// Copyright Mikko Keinänen 2010
//
// This file is part of EmuFront.
//
//
// EmuFront is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Foobar is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#ifndef EMUFRONTDIALOG_H
#define EMUFRONTDIALOG_H
#include <QDialog>

class QErrorMessage;

class EmuFrontDialog : public QDialog
{
        Q_OBJECT
public:
        EmuFrontDialog(QWidget *parent);
protected:
        QErrorMessage *errorMessage;
signals:
        void dbUpdated();

};


#endif // EMUFRONTDIALOG_H
