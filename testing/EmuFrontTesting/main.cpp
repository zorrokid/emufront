#include "platformtest.h"
#include "mediatypetest.h"

int main(int argc, char *argv[])
{
    PlatformTest plfTest;
    QTest::qExec(&plfTest, argc, argv);

    MediaTypeTest mtTest;
    QTest::qExec(&mtTest, argc, argv);

    // More tests here...

    return 0;
}
