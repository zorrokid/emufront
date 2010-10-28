#include <QtGui>
#include "emufrontfileobjectdialog.h"
#include "../db/dbemufrontfileobject.h"

EmuFrontFileObjectDialog::EmuFrontFileObjectDialog(QWidget *parent) :
    DbObjectDialog(parent)
{
}

void EmuFrontFileObjectDialog::setColumnsHidden()
{
    hiddenColumns << DbEmuFrontFileObject::EmuFrontFileObject_Id;
    hiddenColumns << DbEmuFrontFileObject::EmuFrontFileObject_FileCheckSum;
    hiddenColumns << DbEmuFrontFileObject::EmuFrontFileObject_FileType;
    hiddenColumns << DbEmuFrontFileObject::EmuFrontFileObject_FileUpdateTime;
    hiddenColumns << DbEmuFrontFileObject::EmuFrontFileObject_FileId;
    hiddenColumns << DbEmuFrontFileObject::EmuFrontFileObject_FileSize;
}
