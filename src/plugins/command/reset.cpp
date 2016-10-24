#include <QStandardPaths>
#include <base/command_help.h>
#include <base/publish.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include "reset.h"
#include "set_folder.h"

using namespace base;

namespace command {

PUBLISH(reset)
HELP(reset, "reset folders", "reset folder paths to default")

reset::reset(QString name, QObject *parent)
    : Command(name, parent)
{
}

void reset::doExecute() {
    int n = _args.size();
    if (n != 2 || _args.at(1) != "folders") {
        dialog().error("Write: 'reset folders'");
        return;
    }
    QString documents = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0),
            home = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).at(0);
    environment().dir(Environment::Work, documents + "/UniSim");
    environment().dir(Environment::Input, "./input");
    environment().dir(Environment::Output, "./output");
    environment().dir(Environment::Notepad, home + "/AppData/Roaming/Notepad++");
    environment().dir(Environment::Atom, home + "/.atom/packages/language-boxes/grammars");
    set_folder::showAllFolders();
}


}
