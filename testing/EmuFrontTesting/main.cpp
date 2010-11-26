#include "emufrontobjecttest.h"
#include "platformtest.h"
#include "mediatypetest.h"
//#include "platformnamedialogtest.h"

int main(int argc, char *argv[])
{
    // Needed QApplication for widget tests
    //QApplication app(argc, argv);

    EmuFrontObjectTest efoTst;
    QTest::qExec(&efoTst, argc, argv);

    PlatformTest plfTest;
    QTest::qExec(&plfTest, argc, argv);

    MediaTypeTest mtTest;
    QTest::qExec(&mtTest, argc, argv);

    //PlatformNameDialogTest plfDlgTest;
    //QTest::qExec(&plfDlgTest, argc, argv);

    // More tests here...

    //return app.exec();
}
