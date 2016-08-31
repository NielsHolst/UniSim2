#include <base/command_help.h>
#include <base/copy_folder.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "reconfigure.h"

using namespace base;

namespace command {

PUBLISH(reconfigure)
HELP(reconfigure, "reconfigure", "reconfigures all standard folders and files")

reconfigure::reconfigure(QString name, QObject *parent)
    : Command(name, parent)
{
}

void reconfigure::doExecute() {
    QString msg = "Copying default data files:\n" +
                  source().absolutePath() +
                  " -> " +
                   destination().absolutePath() +"...";
    dialog().information(msg);
    copyFolder(source(), destination());
}

QDir reconfigure::source() {
    QDir dir = QDir(environment().homePath());
    QString path("../UniversalSimulator/data");
    bool ok = dir.cd(path);
    if (!ok)
        ThrowException("Cannot find default data folder").
                value(environment().homePath() + "/" + path);
    return dir;
}

QDir reconfigure::destination() {
    return QDir(environment().homePath());
}

}
