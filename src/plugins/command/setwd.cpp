#include <console/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "setwd.h"

using namespace base;
using console::Environment;

namespace command {

PUBLISH(Setwd)

Setwd::Setwd(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(Setwd);
}

void Setwd::execute(Environment *env) {
    checkArgsNumber(2, 2);
    QString arg1 =_args.at(1);
    QDir dir(arg1.replace("\\", "/"));
    bool ok = dir.makeAbsolute();
    if (!ok) {
        QString msg("Command 'setwd' cannot set path to '%1'");
        throw Exception(msg.arg(arg1));
    }
    env->state.workingDirectory = dir;
}

}
