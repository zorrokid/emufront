#include <QtGui>
#include "comboboxdelegate.h"
#include "emufrontquerymodel.h"
#include "emufrontobject.h"

ComboBoxDelegate::ComboBoxDelegate(int column, EmuFrontQueryModel *model, QWidget *parent) :
    QStyledItemDelegate(parent), column(column), model(model)
{ }

void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() != column) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    int objid = index.model()->data(index,  Qt::DisplayRole).toInt();
    EmuFrontObject *efo = model->getObject(objid);
    QString txt = efo->getName();
    painter->save();
    //initStyleOption(&option, index);
    painter->drawText(option.rect, txt);
    painter->restore();
}

//QSize ComboBoxDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const { }

QWidget* ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() != column) {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }

    QComboBox *editor = new QComboBox(parent);
    editor->setEditable(false);
    editor->setModel(model);
    connect(editor, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
}

void ComboBoxDelegate::commitAndCloseEditor()
{
}

