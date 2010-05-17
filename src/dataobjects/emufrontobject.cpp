#include "emufrontobject.h"

EmuFrontObject::EmuFrontObject() : id(-1), name(0)
{
}

/*EmuFrontObject::EmuFrontObject(const EmuFrontObject &ob)
    : id(ob.id), name(ob.name)
{
    // no need to perform deep copy here, see:
    // http://doc.trolltech.com/4.0/shclass.html
}*/

EmuFrontObject::EmuFrontObject(int id, QString name)
    : id(id), name(name)
{}

/*EmuFrontObject::~EmuFrontObject()
{
}

EmuFrontObject& EmuFrontObject::operator =(const EmuFrontObject &ob)
{
    if (this == &ob) return (*this);
    id = ob.id;
    name = ob.name;
    return (*this);
}*/

