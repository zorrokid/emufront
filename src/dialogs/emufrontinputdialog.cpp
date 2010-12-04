#include "emufrontinputdialog.h"
#include <QMap>
#include "emufrontobject.h"

EmuFrontInputDialog::EmuFrontInputDialog(QWidget *parent, Qt::WindowFlags flags)
        : QInputDialog(parent, flags)
{
}

EmuFrontObject* EmuFrontInputDialog::getItem(QWidget *parent, const QString &title, const QString &label,
                               const QList<EmuFrontObject*> &items, int current, bool editable,
                               bool *ok, Qt::WindowFlags flags)
{
    QStringList l;
    QMap<QString, EmuFrontObject*> map;
    foreach(EmuFrontObject *efo, items){
        l << efo->getName();
        map[efo->getName()] = efo;
    }
    QString sel = QInputDialog::getItem(parent, title, label, l, current, editable, ok, flags);
    return map.value(sel, 0);
}
