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
#include <QtGui>
#include <QSqlQueryModel>
#include "comboboxdelegate.h"
#include "emufrontquerymodel.h"
#include "emufrontobject.h"

ComboBoxDelegate::ComboBoxDelegate(QSqlQueryModel *cbmodel, int cbmodelIdColumn, int cbmodelDisplayColumn, QWidget *parent) :
    QStyledItemDelegate(parent), cbmodel(cbmodel), cbmodelIdColumn(cbmodelIdColumn), cbmodelDisplayColumn(cbmodelDisplayColumn)
{ }

void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // get the item id from the hosting view's model
    int objid = index.model()->data(index,  Qt::DisplayRole).toInt();

    // find a matching object from the editor's model
    QModelIndex startInd = cbmodel->index(0, cbmodelIdColumn);
    QModelIndexList indList = cbmodel->match(startInd, Qt::DisplayRole, objid);
    // If an object was found in the editor's model create an index to the editor's model
    QModelIndex ind = indList.empty() ?
        QModelIndex() :
        cbmodel->index(indList.first().row(), cbmodelDisplayColumn);

    // Get the name field of an object using the created index
    QString txt = ind.isValid() ?
        cbmodel->data(ind).toString() : "";

    // Render the name text
    painter->save();
    painter->drawText(option.rect, txt);
    painter->restore();
}

//QSize ComboBoxDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const { }

QWidget* ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setEditable(false);
    editor->setModel(cbmodel);
    editor->setModelColumn(cbmodelDisplayColumn);
    connect(editor, SIGNAL(currentIndexChanged(int)), this, SLOT(commitAndCloseEditor(int)));
    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // set the correct item selected for the editor:
    // get the item id from the parent view's model
    int id = index.model()->data(index, Qt::DisplayRole).toInt();
    QComboBox *cbox = qobject_cast<QComboBox *>(editor);
    // create a start index from combobox model
    QModelIndex startInd = cbmodel->index(0, cbmodelIdColumn);
    // search an index for selected item in parent view
    QModelIndexList indList = cbmodel->match(startInd, Qt::DisplayRole, id);
    // if no match set no item selected
    int ind = indList.empty() ? -1 : indList.first().row();
    cbox->setCurrentIndex(ind);
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *cbox = qobject_cast<QComboBox *>(editor);
    if (cbox->currentIndex() == -1) return;
    QModelIndex mi = cbmodel->index(cbox->currentIndex(), cbmodelIdColumn);
    int id = cbmodel->data(mi).toInt();
    model->setData(index, id);
}

void ComboBoxDelegate::commitAndCloseEditor(int)
{
    QComboBox *editor = qobject_cast<QComboBox *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

