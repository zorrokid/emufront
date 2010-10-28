#ifndef EMUFRONTFILEOBJECTDIALOG_H
#define EMUFRONTFILEOBJECTDIALOG_H

#include "dbobjectdialog.h"

class EmuFrontFileObjectDialog : public DbObjectDialog
{
    Q_OBJECT
public:
    explicit EmuFrontFileObjectDialog(QWidget *parent = 0);

protected:
    void setColumnsHidden();
};

#endif // EMUFRONTFILEOBJECTDIALOG_H
