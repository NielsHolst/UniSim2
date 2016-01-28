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

void cd::execute(DialogBase *dialog, Environment *env) {
    QDir current = env->state.dir;

    QString dirName;
    switch (_args.size()) {
    case 1:
        dialog->information(current.absolutePath());
        break;
    case 2:
        dirName = _args.at(1);
        if ( current.cd(dirName) ) {
            env->state.dir = current;
            dialog->information(env->state.dir.absolutePath());
        }
        else {
            QString msg("Unknown directory: '%1'\nPath unchanged: %2");
            dialog->error(msg.arg(dirName).arg(env->state.dir.absolutePath()));
        }
        break;
    default:
        dialog->error("Command 'cd' takes at most 1 argument");
    }
}

}
