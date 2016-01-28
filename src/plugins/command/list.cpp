#include <base/command.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "list.h"

using namespace base;

namespace command {

PUBLISH(list)

list::list(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(list);
}

void list::execute(DialogBase *dialog, Environment *) {
    dialog->error("Nothing to list");
}

}
