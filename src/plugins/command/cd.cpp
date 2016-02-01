#include <base/command.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "cd.h"

using namespace base;

namespace command {

PUBLISH(cd)

cd::cd(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(cd);
}

void cd::execute() {
    Environment &env(environment());
    DialogBase &dia(dialog());
    QDir current = env.state.dir;

    QString dirName;
    switch (_args.size()) {
    case 1:
        dia.information(current.absolutePath());
        break;
    case 2:
        dirName = _args.at(1);
        if (dirName == "~") {
            env.state.dir = QDir::home();
            dia.information(env.state.dir.absolutePath());
        }
        else if ( current.cd(dirName) ) {
            env.state.dir = current;
            dia.information(env.state.dir.absolutePath());
        }
        else {
            QString msg("Unknown directory: '%1'\nPath unchanged: %2");
            dia.error(msg.arg(dirName).arg(env.state.dir.absolutePath()));
        }
        break;
    default:
        dia.error("Command 'cd' takes at most 1 argument");
    }
}

}
