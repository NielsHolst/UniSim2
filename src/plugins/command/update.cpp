#include <base/box.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "update.h"

using namespace base;

namespace command {

PUBLISH(update)

update::update(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(update);
}

void update::execute() {
    Box *root = environment().state.root;
    if (_args.size() > 1) {
        dialog().error("Command 'update' takes no arguments");
    }
    else if (root) {
        try {
            root->updateFamily();
        }
        catch (Exception &ex) {
            dialog().error(root->objectName() + " interrupted\n" + ex.what());
        }
        dialog().information(root->objectName() + " finished");
    }
    else
        dialog().error("No box loaded");
}

}
