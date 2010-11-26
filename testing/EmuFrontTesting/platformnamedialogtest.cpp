#include "platformnamedialogtest.h"

void PlatformNameDialogTest::testLineEdit()
{
    Platform *plf = new Platform(1, "");
    PlatformNameDialog dlg;
    dlg.setDataObject(plf);
    dlg.show();
    QTest::qWait(2000);

    /*QTest::mouseClick(&dlg, Qt::LeftButton);*/
    QTest::keyClick(&dlg, Qt::Key_N, Qt::AltModifier);
    QTest::keyClicks(&dlg, "hello world");
    QTest::qWait(1000);
    QTest::keyClick(&dlg, Qt::Key_Enter);
    QCOMPARE(plf->getName(), QString("hello world"));
}
