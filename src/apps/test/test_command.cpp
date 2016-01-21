#include <QDir>
#include <QStringList>
#include <boxes/mega_factory.h>
#include <command/command.h>
#include <console/environment.h>
#include "test_command.h"

using namespace boxes;
using namespace command;
using namespace console;

void TestCommand::testSetwdCreate() {
    Command *setwd = MegaFactory::create<Command>("Setwd", "setwd", 0);
    QVERIFY(setwd);
    delete setwd;
}
void TestCommand::testSetwdExecute() {
    Command *setwd = MegaFactory::create<Command>("Setwd", "setwd", 0);
    QVERIFY(setwd);
    setwd->arguments(QStringList() << "setwd" << QDir::rootPath());

    Environment env;
    try {
        env.execute(setwd);
    }
    catch (Exception &ex) {
        QFAIL(ex.what());
    }
    QCOMPARE(env.state.workingDirectory, QDir(QDir::rootPath()));

    delete setwd;
}

