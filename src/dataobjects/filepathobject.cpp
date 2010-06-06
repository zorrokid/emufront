#include "filepathobject.h"
#include "setup.h"

FilePathObject::FilePathObject() : EmuFrontFileObject(), setup(0)
{
}

FilePathObject::FilePathObject(int id, QString name, QString filename, int filetype)
    : EmuFrontFileObject(id, name, filename, filetype), setup(0) {}

 FilePathObject::FilePathObject(int id, QString name, QString filename,
     int filetype, Setup *setup)
    : EmuFrontFileObject(id, name, filename, filetype), setup(setup) {}

FilePathObject::~FilePathObject()
{
    if (setup) delete setup;
}

FilePathObject::FilePathObject(const FilePathObject &fpobj)
    : EmuFrontFileObject(fpobj.id, fpobj.name, fpobj.filename, fpobj.filetype)
{
    Setup *s = fpobj.setup;
    setup = new Setup(*s);
}

FilePathObject& FilePathObject::operator =(const FilePathObject &fpobj)
{
    if (this == &fpobj) return *this;
    id = fpobj.id;
    name = fpobj.name;
    filename = fpobj.filename;
    filetype = fpobj.filetype;
    if (setup) delete setup;
    Setup *sup = fpobj.setup;
    setup = new Setup(*sup);
    return (*this);
}

Setup* FilePathObject::getSetup() const
{ return setup; }
void FilePathObject::setSetup(Setup *sup)
{ setup = sup; }
