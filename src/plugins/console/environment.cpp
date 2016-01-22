#include <QDir>
#include <command/command.h>
#include "environment.h"

using namespace base;
using command::Command;

namespace console {

Environment::Environment()
{
    state.workingDirectory = QDir::homePath();
    state.workingDirectory.makeAbsolute();
}

void Environment::execute(Command *com) {
    com->execute(this);
}

}
