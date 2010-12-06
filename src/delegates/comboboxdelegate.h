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
