#ifndef EMUFRONTOBJECT_H
#define EMUFRONTOBJECT_H

#include <QObject>

class EmuFrontObject : public QObject
{
public:
    EmuFrontObject();
    EmuFrontObject(int id, QString name);

    // No need for these as long we use QString (see Implicit Data Sharing)
    //EmuFrontObject(const EmuFrontObject &);
    //~EmuFrontObject();
    //EmuFrontObject &operator=(const EmuFrontObject &);

    const QString getName() const
    { return name; }
    int getId() const
    { return id; }
    void setName(QString name)
    { this->name = name; };
    void setId(int id)
    { this->id = id; }

protected:
    int id;
    QString name;
};

#endif // EMUFRONTOBJECT_H
