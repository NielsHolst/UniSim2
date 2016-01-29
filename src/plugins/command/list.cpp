#include <base/dialog.h>
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

void list::execute() {
    dialog().error("Nothing to list");
}

}
