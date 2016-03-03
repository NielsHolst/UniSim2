#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "load.h"
#include "run.h"

using namespace base;

namespace command {

PUBLISH(run)
HELP(run, "run", "runs the root box")

run::run(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(run);
}

void run::doExecute() {
    Environment &env(environment());
    if (_args.size() > 2)
        throw Exception("Command 'run' takes at most one argument");
    doLoad();
    if (!env.state.command->hasError())
        doRun();
}

void run::doLoad() {
    Command *loadCom = new load("load", this);
    loadCom->execute();
}

void run::doRun() {
    Box *root = environment().state.root;
    Q_ASSERT(root);
    try {
        root->run();
    }
    catch (Exception &ex) {
        throw Exception("Run: " + root->objectName() + " interrupted\n" + ex.fullText());
    }
}

}
