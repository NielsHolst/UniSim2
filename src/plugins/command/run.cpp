#include <QTime>
#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/path.h>
#include <base/port.h>
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
    if (_args.size() > 2)
        ThrowException("Command 'run' takes at most one argument");
    dialog().resetErrorCount();
    doLoad();
    if (dialog().errorCount() == 0)
        doRun();
}

void run::doLoad() {
    QStringList com;
    com << "load";
    if (_args.size() == 2)
        com << _args[1];
    Command::submit(com, this);
}

void run::doRun() {
    _root = environment().root();
    if (!_root) {
        dialog().error("Nothing to run");
        return;
    }
    _root->run();

    const Port *hasErrorPort = _root->peakPort("hasError");
    bool noError = !(hasErrorPort && hasErrorPort->value<bool>());

    QString msg = noError ? "Finished" : "Interrupted";
    msg += messageTime();
    msg += message("iteration");
    msg += message("step");

    QString errorMsg;

    if (noError)
        dialog().information(msg);
    else
        dialog().error(msg + errorMessage());
}

QString run::messageTime() const {
    const Port *port = _root->peakPort("executionTime");
    if (!port)
        return "";
    int dt = port->value<int>();
    QString units = "msecs";
    if (dt > 5000) {
        dt /= 1000;
        units = "secs";
    }
    QString s{" after %1 %2"};
    return s.arg(dt).arg(units);
}

QString run::message(QString name) const {
    Port *i = _root->peakPort(name),
         *n = _root->peakPort(name+"s");
    if (!i || !n || n->value<int>() == 1)
        return "";

    QString s = " in " + name + " %1/%2";
    return s. arg(i->value<int>()-1). arg(n->value<int>());
}

QString run::errorMessage() const {
    const Port *port = _root->peakPort("errorMsg");
    return port ? ("\n" + port->value<QString>()) : QString();
}

}
