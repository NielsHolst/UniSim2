#include <base/box.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "run.h"

using namespace base;

namespace command {

PUBLISH(run)

run::run(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(run);
}

void run::execute() {
    Box *root = environment().state.root;
    if (_args.size() > 1) {
        dialog().error("Command 'run' takes no arguments");
    }
    else if (root) {
        try {
            root->run();
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
