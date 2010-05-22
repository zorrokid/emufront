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

#ifndef PLATFORMNAMEDIALOG_H
#define PLATFORMNAMEDIALOG_H

#include "namedialog.h"
#include "../dataobjects/platform.h"

class PlatformNameDialog : public NameDialog
{
	Q_OBJECT

public:
    PlatformNameDialog(QWidget *parent = 0, Platform * = 0);
    virtual void setDataObject(EmuFrontObject*);

protected:
    virtual void setDataObject(QString name);
};

#endif
