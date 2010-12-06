#include <QtGui>
#include <QSqlQueryModel>
#include "comboboxdelegate.h"
#include "emufrontquerymodel.h"
#include "emufrontobject.h"

ComboBoxDelegate::ComboBoxDelegate(int viewColumn, QSqlQueryModel *cbmodel, int cbmodelIdColumn, int cbmodelDisplayColumn, QWidget *parent) :
    QStyledItemDelegate(parent), viewColumn(viewColumn), cbmodel(cbmodel), cbmodelIdColumn(cbmodelIdColumn), cbmodelDisplayColumn(cbmodelDisplayColumn)
{ }

void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() != viewColumn) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    // get the item id from the hosting view's model
    int objid = index.model()->data(index,  Qt::DisplayRole).toInt();

    // find matching object from QComboBox's model
    QModelIndex startInd = cbmodel->index(0, cbmodelIdColumn);
    QModelIndexList indList = cbmodel->match(startInd, Qt::DisplayRole, objid);


    QModelIndex ind = indList.empty() ?
        QModelIndex() :
        cbmodel->index(indList.first().row(), cbmodelDisplayColumn);

    QString txt = ind.isValid() ?
        cbmodel->data(ind).toString() : "";

    painter->save();
    //initStyleOption(&option, index);
    painter->drawText(option.rect, txt);
    painter->restore();
}

//QSize ComboBoxDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const { }

QWidget* ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() != viewColumn) {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }

    QComboBox *editor = new QComboBox(parent);
    editor->setEditable(false);
    editor->setModel(cbmodel);
    editor->setModelColumn(cbmodelDisplayColumn);
    connect(editor, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() != viewColumn) {
        return QStyledItemDelegate::setEditorData(editor, index);
    }

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
    if (index.column() != viewColumn) {
        return QStyledItemDelegate::setModelData(editor, model, index);
    }
    QComboBox *cbox = qobject_cast<QComboBox *>(editor);
    if (cbox->currentIndex() == -1) return;
    QModelIndex mi = cbmodel->index(cbox->currentIndex(), cbmodelIdColumn);
    int id = cbmodel->data(mi).toInt();
    model->setData(index, id);
}

void ComboBoxDelegate::commitAndCloseEditor()
{
    QComboBox *editor = qobject_cast<QComboBox *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

