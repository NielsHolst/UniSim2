#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "update.h"

using namespace base;

namespace command {

PUBLISH(update)
HELP(update, "update", "updates the root box")

update::update(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(update);
}

void update::doExecute() {
    Box *root = environment().state.root;
    if (_args.size() > 1) {
        throw Exception("Command 'update' takes no arguments");
    }
    else if (root) {
        try {
            root->updateFamily();
        }
        catch (Exception &ex) {
            throw Exception("Update: " + root->objectName() + " interrupted\n" + ex.what());
        }
        dialog().information("Update: " + root->objectName() + " finished");
    }
    else
        throw Exception("No box loaded");
}

}
