#include <QDir>
#include <command/command.h>
#include "environment.h"

using namespace boxes;
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
