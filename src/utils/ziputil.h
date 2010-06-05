#ifndef ZIPUTIL_H
#define ZIPUTIL_H
#include <iostream>
#include <string>

using namespace std;

class ZipUtil
{
public:
    ZipUtil();
    static void zipContents(string filePath);
};

#endif // ZIPUTIL_H
