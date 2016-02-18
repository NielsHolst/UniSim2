#include "command.h"
#include "dialog.h"
#include "environment.h"
#include "exception.h"
#include "general.h"

namespace base {

QStringList Command::_help;

Command::Command(QString name, QObject *parent)
    : QObject(parent), _error(false)
{
    Class(Command);
    setObjectName(name);
}

void Command::arguments(QStringList args) {
    _args = args;
}

QStringList Command::arguments() const {
    return _args;
}

void Command::execute() {
    try {
        doExecute();
        _error = false;
    }
    catch (Exception &ex) {
        _error = true;
        dialog().error(ex.what());
    }
    environment().state.command = this;
}

bool Command::hasError() const {
    Command *parentCommand = dynamic_cast<Command*>(parent());
    return _error || (parentCommand && parentCommand->hasError());
}

void Command::helpText(QString help) {
    _help << help;
}

QStringList Command::help() {
    _help.sort();
    return _help;
}

}
