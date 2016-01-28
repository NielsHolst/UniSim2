#include <QDir>
#include <QStringList>
#include <base/command.h>
#include <base/dialog_stub.h>
#include <base/environment.h>
#include <base/mega_factory.h>
#include "test_command.h"

using namespace base;

void TestCommand::testSetwdCreate() {
    Command *cd = MegaFactory::create<Command>("cd", "cd", 0);
    QVERIFY(cd);
    delete cd;
}
void TestCommand::testSetwdExecute() {
    Command *cd = MegaFactory::create<Command>("cd", "cd", 0);
    QVERIFY(cd);
    cd->arguments(QStringList() << "cd" << QDir::rootPath());

    DialogStub *dialog = new DialogStub(0);
    Environment env;
    try {
        cd->execute(dialog, &env);
    }
    catch (Exception &ex) {
        QFAIL(ex.what());
    }
    QCOMPARE(dialog->getInformation(), QDir::rootPath());

    delete cd;
}

