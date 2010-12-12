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
*/#ifndef STRINGLISTWIDGET_H
#define STRINGLISTWIDGET_H

#include <QWidget>

class QListWidget;
class QListWidgetItem;
class QPushButton;

class StringListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StringListWidget(QWidget *parent = 0, bool sort = true, int sortIndex = 0);
    QStringList getItems();
    void setItems(QStringList);
    void clear();

signals:
    void stringListUpdated();

protected:
    virtual bool confirmInput(const QString &) const;

private slots:
    void addClicked();
    void removeClicked();
    void itemUpdated(QListWidgetItem *);

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
