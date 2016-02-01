#include <base/box_output.h>
#include <base/dialog.h>
#include <base/environment.h>
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
    Box *root = environment().state.root;
    if (root) {
        BoxOutput output(root, BoxOutput::Indented);
        dialog().information(output.asText());
    }
    else
        dialog().information("No box loaded");
}

}
