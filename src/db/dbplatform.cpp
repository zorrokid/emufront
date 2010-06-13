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
// Foobar is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#include "dbplatform.h"

//QString DbPlatform::tableName = DbPlatform::DB_TABLE_NAME_PLATFORM;

DbPlatform::DbPlatform(QObject *parent) : DbEmuFrontFileObject(parent)
{
    tableName = DbPlatform::DB_TABLE_NAME_PLATFORM;
 }

EmuFrontObject* DbPlatform::createEmuFrontFileObject(int id, QString name, EmuFrontFile *f)
{   return new Platform(id, name, f); }
