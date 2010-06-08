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

#ifndef STRINGLISTWIDGET_H
#define STRINGLISTWIDGET_H

#include <QWidget>

//class QStringListModel;
class QListWidget;
class QPushButton;

class StringListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StringListWidget(QWidget *parent = 0, bool sort = true, int sortIndex = 0);
    QStringList getItems();
    void setItems(QStringList);

signals:
    void stringListUpdated();

private slots:
    void addClicked();
    void removeClicked();

private:
    void initUi();
    void connectSignals();
    //QStringListModel *model;
    QListWidget *stringList;
    QPushButton *btnAdd;
    QPushButton *btnRemove;
    bool sort;
    int sortIndex;
};

#endif // STRINGLISTWIDGET_H
