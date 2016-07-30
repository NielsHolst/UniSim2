#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/publish.h>
#include "profile.h"

using namespace base;

namespace command {

PUBLISH(profile)
HELP(profile, "profile", "writes execution profile to output file")

profile::profile(QString name, QObject *parent)
    : Command(name, parent)
{
}

void profile::doExecute() {
    Box *root = environment().root();
    if (_args.size() > 2) {
        throw Exception("Command 'profile' takes no arguments");
    }
    else if (root) {
        QFile file;
        environment().openOutputFile(file, ".txt");
        file.write(qPrintable(root->profileReport()));
        file.close();
        environment().incrementFileCounter();
    }
    else
        throw Exception("No box loaded");
}

}
