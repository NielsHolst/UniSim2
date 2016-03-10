#include <base/box_output.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "list.h"

using namespace base;

namespace command {

PUBLISH(list)
HELP(list, "list", "lists box structure starting from root")

list::list(QString name, QObject *parent)
    : Command(name, parent)
{
}

void list::doExecute() {
    Box *root = environment().state.root;
    if (root) {
        BoxOutput output(root, BoxOutput::Indented);
        dialog().information(output.asText());
    }
    else
        throw Exception("No box loaded");
}

}
