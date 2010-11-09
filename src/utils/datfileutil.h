#ifndef DATFILEUTIL_H
#define DATFILEUTIL_H

#include <QWidget>
#include <QtXml>

class DatFileUtil : public QWidget
{
    Q_OBJECT
public:
    explicit DatFileUtil(QWidget *parent = 0);
    void open();

signals:

public slots:

private:

};

#endif // DATFILEUTIL_H
