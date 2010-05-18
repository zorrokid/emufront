#ifndef PLATFORM_H
#define PLATFORM_H

#include "emufrontobject.h"

class Platform : public EmuFrontObject
{
public:
    Platform();
    Platform(int id, QString name, QString filename);
    // No need for these as long we use QString (see Implicit Data Sharing)
    /*Platform(const Platform &);
    Platform &operator=(const Platform &);
    virtual ~Platform();*/
    const QString getFilename() const
    { return filename; }
    void setFilename(QString filename)
    { this->filename = filename; }

private:
    QString filename;
};

#endif // PLATFORM_H
