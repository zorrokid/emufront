#include "platform.h"

Platform::Platform() : EmuFrontObject()
{
}

Platform::Platform(int id, QString name, QString filename)
        : EmuFrontObject(id, name), filename(filename)
{
}

// we make deep copies of name and filename strings
/*Platform::Platform(const Platform &pl) : EmuFrontObject(pl.id, pl.name), filename(pl.filename)
{
    // no need to perform deep copy here, see:
    // http://doc.trolltech.com/4.0/shclass.html
}*/
