#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "load.h"
#include "run.h"
#include "save_output.h"

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
    if (!env.state.command->hasError() && environment().state.autosave)
        doSave();
}

void run::doLoad() {
    QString fileName = (_args.size() == 1) ?
                       environment().state.latestLoadArg :
                       _args.at(1);
    if (fileName.isEmpty())
        throw Exception("No box loaded");
    Command *loadCom = new load("load", this);
    loadCom->arguments(QStringList() << "load" << fileName);
    loadCom->execute();
}

void run::doRun() {
    Box *root = environment().state.root;
    Q_ASSERT(root);
    try {
        root->run();
    }
    catch (Exception &ex) {
        throw Exception("Run: " + root->objectName() + " interrupted\n" + ex.what());
    }
    dialog().information("Run: " + root->objectName() + " finished");
}

void run::doSave() {
    Command *save = new save_output("autosave_output", this);
    save->arguments(QStringList() << "save" << "output");
    save->execute();
}

}
