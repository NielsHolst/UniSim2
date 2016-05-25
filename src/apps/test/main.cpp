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

    QString &loadArg(base::environment().state.latestLoadArg),
            saveLoadArg = loadArg;
    loadArg = "test.box";

    QDir &dirInput(base::environment().state.dir.input),
         saveDirInput = dirInput;
    dirInput = QDir();
    dirInput.cd("input");

    int result = AutoTest::run(argc, argv);
    QString msg = (result==0) ? "All tests passed" : "One or more tests failed";
    std::cout << "\n\n" << qPrintable(msg) << "\n";

    loadArg = saveLoadArg;
    dirInput = saveDirInput;

    dialog->deleteLater();
    return result;
}
