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
    //virtual ~EmuFrontObject();
    //EmuFrontObject &operator=(const EmuFrontObject &);

    virtual const QString getName() const
    { return name; }
    virtual int getId() const
    { return id; }
    virtual void setName(QString name)
    { this->name = name; };
    virtual void setId(int id)
    { this->id = id; }

protected:
    int id;
    QString name;
};

#endif // EMUFRONTOBJECT_H
