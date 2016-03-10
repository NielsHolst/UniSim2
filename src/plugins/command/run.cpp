#include <QTime>
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
    QTime time;
    time.start();
    try {
        root->run();
    }
    catch (Exception &ex) {
        throw Exception("Run: " + root->objectName() + " interrupted\n" + ex.fullText());
    }
    int dt = time.elapsed();
    QString units = "msecs";
    if (dt > 5000) {
        dt /= 1000;
        units = "secs";
    }
    QString info{"Finished after %1 %2"};
    dialog().information(info.arg(dt).arg(units));
}

}
