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

#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QStyledItemDelegate>

class QSqlQueryModel;

class ComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ComboBoxDelegate(
        QSqlQueryModel *,
        int cbmodelIdColumn,
        int cbmodelDisplayColumn,
        QWidget *parent = 0
    );

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    //QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private slots:
    void commitAndCloseEditor(int);
private:
    QSqlQueryModel *cbmodel;
    int cbmodelIdColumn;
    int cbmodelDisplayColumn;
};

#endif // COMBOBOXDELEGATE_H
