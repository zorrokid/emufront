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
#include "stringlistdelegate.h"
#include "fileextensionwidget.h"

StringListDelegate::StringListDelegate(QString separator, QObject *parent) :
    QStyledItemDelegate(parent), separator(separator)
{
}

/*void StringListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString str = index.model()->data(index, Qt::DisplayRole).toString();
    // TODO:...
}*/

QWidget* StringListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StringListWidget *editor = new StringListWidget(parent);
    QString str = index.model()->data(index, Qt::DisplayRole).toString();
    editor->setItems(str.split(separator, QString::SkipEmptyParts));
    connect(editor, SIGNAL(stringListUpdated()), this, SLOT(commitAndCloseEditor()));
    return editor;
}

void StringListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str = index.model()->data(index, Qt::DisplayRole).toString();
    StringListWidget *strListWdg = qobject_cast<StringListWidget *>(editor);
    strListWdg->setItems(str.split(separator, QString::SkipEmptyParts));
}

void StringListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    StringListWidget *strListWdg = qobject_cast<StringListWidget *>(editor);
    QStringList ls = strListWdg->getItems();
    model->setData(index, ls.empty() ? "" : ls.join(separator));
}

void StringListDelegate::commitAndCloseEditor()
{
    StringListWidget *editor = qobject_cast<StringListWidget *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

QSize StringListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize sz(300,300);
    return sz;
}
