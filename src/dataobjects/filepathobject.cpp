#include "filepathobject.h"

FilePathObject::FilePathObject() : EmuFrontFileObject(), platform(0), mediaType(0)
{
}

FilePathObject::FilePathObject(int id, QString name, QString filename, int filetype)
    : EmuFrontFileObject(id, name, filename, filetype), platform(0), mediaType(0)
 {}

 FilePathObject::FilePathObject(int id, QString name, QString filename,
     int filetype, Platform *plf, MediaType *med)
    : EmuFrontFileObject(id, name, filename, filetype), platform(plf), mediaType(med)
 {}

FilePathObject::~FilePathObject()
{
    if (platform) delete platform;
    if (mediaType) delete mediaType;
}

FilePathObject::FilePathObject(const FilePathObject &fpobj)
    : EmuFrontFileObject(fpobj.id, fpobj.name, fpobj.filename, fpobj.filetype)
{
    // Note: no need to deep copy members of type QString
    // QString uses Implicit Data Sharing (http://doc.trolltech.com/4.0/shclass.html)
    Platform *p = fpobj.platform;
    platform = new Platform(p->getId(), p->getName(), p->getFilename());
    MediaType *mt = fpobj.mediaType;
    mediaType = new MediaType(mt->getId(), mt->getName(), mt->getFilename());
}

FilePathObject& FilePathObject::operator =(const FilePathObject &fpobj)
{
    if (this == &fpobj) return *this;
    id = fpobj.id;
    name = fpobj.name;
    filename = fpobj.filename;
    filetype = fpobj.filetype;
    if (platform) delete platform;
    Platform *p = fpobj.platform;
    platform = new Platform(p->getId(), p->getName(), p->getFilename());
    if (mediaType) delete mediaType;
    MediaType *mt = fpobj.mediaType;
    mediaType = new MediaType(mt->getId(), mt->getName(), mt->getFilename());
    return (*this);
}
