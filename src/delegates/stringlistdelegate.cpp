/*
** EmuFront
** Copyright 2010-2011 Mikko Keinänen
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
#include <QPainter>
#include <QDebug>

StringListDelegate::StringListDelegate(QString separator, QObject *parent) :
    QStyledItemDelegate(parent), separator(separator)
{
}

QWidget* StringListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    StringListWidget *editor = new StringListWidget(parent);
	editor->setFixedSize(WIDTH, HEIGHT);
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
	
	qDebug() << "sizeHinta";
    QSize sz(WIDTH, HEIGHT);
    return sz;
}

void StringListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (option.state & QStyle::State_Editing) {
		QRect rc(option.rect.x(), option.rect.y(), WIDTH, HEIGHT);
		painter->fillRect(rc, option.palette.highlight());
	}
	else {
		painter->save();
		QRect rc(option.rect.x(), option.rect.y(), WIDTH, HEIGHT);
		painter->fillRect(rc, (option.state & QStyle::State_Selected) ? option.palette.highlight() : option.palette.light() );
		QString str = index.model()->data(index, Qt::DisplayRole).toString();
		painter->setBrush(option.palette.foreground());
		painter->drawText(rc, str);
		painter->restore();
	}
}
