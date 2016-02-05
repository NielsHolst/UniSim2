#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "cd.h"

using namespace base;

namespace command {

PUBLISH(cd)
HELP(cd, "cd", "show working directory")
HELP(cd_path, "cd <path>", "change working directory")

cd::cd(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(cd);
}

void cd::doExecute() {
    Environment &env(environment());
    DialogBase &dia(dialog());
    QDir current = env.state.dir.work;

    QString dirName;
    switch (_args.size()) {
    case 1:
        dia.information(current.absolutePath());
        break;
    case 2:
        dirName = _args.at(1);
        if (dirName == "~") {
            env.state.dir.work = QDir::home();
            dia.information(env.state.dir.work.absolutePath());
        }
        else if ( current.cd(dirName) ) {
            env.state.dir.work = current;
            dia.information(env.state.dir.work.absolutePath());
        }
        else {
            QString msg("Unknown directory: '%1'\nPath unchanged: %2");
            throw Exception(msg.arg(dirName).arg(env.state.dir.work.absolutePath()));
        }
        break;
    default:
        throw Exception("Command 'cd' takes at most 1 argument");
    }
}

}
