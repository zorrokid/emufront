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

#include "stringlistwidget.h"
#include <QtGui>

StringListWidget::StringListWidget(QWidget *parent, bool sort, int sortIndex) :
    QWidget(parent), sort(sort), sortIndex(sortIndex)
{
    initUi();
    connectSignals();
}

void StringListWidget::initUi()
{
    stringList = new QListWidget(this);
    btnAdd = new QPushButton(tr("&Add"), this);
    btnRemove = new QPushButton(tr("&Remove"), this);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(btnAdd);
    rightLayout->addWidget(btnRemove);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(stringList);
    mainLayout->addLayout(rightLayout);

    setLayout(mainLayout);
}

void StringListWidget::connectSignals()
{
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(addClicked()));
    connect(btnRemove, SIGNAL(clicked()), this, SLOT(removeClicked()));
}

void StringListWidget::addClicked()
{
    QString input = QInputDialog::getText(this, tr("Add"), tr("Add new item"));
    if (input.isEmpty()) return;
    stringList->addItem(input);
    stringList->sortItems();
    emit stringListUpdated();
}

void StringListWidget::removeClicked()
{
    qDebug() << "StringListWidget::removeClicked";
    int row = stringList->currentRow();
    if (row >= 0 && row < stringList->count())
    {
        stringList->takeItem(row);
    }
    emit stringListUpdated();
}

QStringList StringListWidget::getItems()
{
    QStringList l;
    for(int i = 0; i < stringList->count(); ++i)
        l << stringList->item(i)->text();
    return l;
}

void StringListWidget::setItems(QStringList list)
{
    stringList->clear();
    foreach(QString s, list)
        stringList->addItem(s);
}
