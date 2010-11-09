#include "datfileutil.h"
#include <QtGui>

DatFileUtil::DatFileUtil(QWidget *parent) :
    QWidget(parent)
{
}

void DatFileUtil::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Dat File"),
        QDir::currentPath(), tr("XML Dat Files (*.xml)"));
    if (fileName.isEmpty()) return;

    QDomDocument doc("datFile");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            qDebug() << qPrintable(e.tagName()) << endl; // the node really is an element.
        }
        n = n.nextSibling();
    }
}
