#include "platformtest.h"
#include "../../src/dataobjects/platform.h"

/* Platforms are equal if the following fields match:
    - id (int)
    - name (QString)
    - filename (QString)
*/
void PlatformTest::equals()
{
    Platform p1(1, "testa");
    Platform p2(1, "test");
    // This should return true
    QVERIFY(p1 != p2);
}
