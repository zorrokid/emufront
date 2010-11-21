#include "platformtest.h"

int main(int argc, char *argv[])
{
    PlatformTest plfTest;
    QTest::qExec(&plfTest, argc, argv);

    // More tests here...

    return 0;
}
