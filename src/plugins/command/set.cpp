#include <base/command.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "set.h"

using namespace base;

namespace command {

PUBLISH(set)

set::set(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(set);
}

void set::execute(DialogBase *dialog, Environment *) {
    dialog->error("Nothing to set");
}

}
