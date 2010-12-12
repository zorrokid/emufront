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
#include "emufrontfile.h"
#include <QDebug>

EmuFrontFile::EmuFrontFile() : EmuFrontObject() { }

EmuFrontFile::EmuFrontFile(int type) : EmuFrontObject(), type(type) { }

EmuFrontFile::EmuFrontFile(int id, QString name, QString checksum, int size, int type)
   : EmuFrontObject(id, name), checkSum(checksum), size(size), type(type) { }

EmuFrontFile::EmuFrontFile(const EmuFrontFile &eff)
    : EmuFrontObject(eff), checkSum(eff.checkSum), size(eff.size), type(eff.type)
{}

EmuFrontFile::~EmuFrontFile()
{
    qDebug() << "EmuFrontFile " << name << " dying.";
}

QString EmuFrontFile::getCheckSum() const
{ return checkSum; }

void EmuFrontFile::setCheckSum(QString s)
{ checkSum = s; }

int EmuFrontFile::getSize() const
{ return size; }

void EmuFrontFile::setSize(int s)
{ size = s; }

void EmuFrontFile::setType(int t)
{ type = t; }

int EmuFrontFile::getType() const
{ return type; }
