#ifndef EMUFRONTINPUTDIALOG_H
#define EMUFRONTINPUTDIALOG_H

#include <QInputDialog>

class EmuFrontObject;
class EmuFrontInputDialog : public QInputDialog
{
public:
    EmuFrontInputDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    static EmuFrontObject* getItem(QWidget *parent, const QString &title, const QString &label,
                           const QList<EmuFrontObject*> &items, int current = 0, bool editable = true,
                           bool *ok = 0, Qt::WindowFlags flags = 0);
};

#endif // EMUFRONTINPUTDIALOG_H
