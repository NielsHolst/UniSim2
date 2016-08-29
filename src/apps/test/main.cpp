#include <iostream>
#include <QCoreApplication>
#include <base/dialog_stub.h>
#include <base/environment.h>
#include <base/object_pool.h>
#include <base/organisation.h>
#include "autotest.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    new base::ObjectPool(&app);
    base::DialogStub *dialog = new base::DialogStub(0);

    QString saveLoadArg = base::environment().latestLoadArg();
    base::environment().latestLoadArg("test.box");

    QDir saveDirInput = base::environment().dir(base::Environment::Input),
         testDirInput = QDir();
    if (!testDirInput.cd("../src/apps/test/input/")) {
        std::cout << "'../src/apps/test/input/' path not found from"
                  << qPrintable(testDirInput.absolutePath());
        return 0;
    }
    base::environment().dir(base::Environment::Input, testDirInput);

    int result = AutoTest::run(argc, argv);
    QString msg = (result==0) ? "All tests passed" : "One or more tests failed";
    std::cout << "\n\n" << qPrintable(msg) << "\n";

    base::environment().latestLoadArg(saveLoadArg);
    base::environment().dir(base::Environment::Input, saveDirInput);

    dialog->deleteLater();
    return result;
}
