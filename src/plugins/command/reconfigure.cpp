#include <QStandardPaths>
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
    QStringList paths =
        QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    if (paths.isEmpty())
        ThrowException("Cannot find app data folder");
    return QDir(paths.at(0));
}

QDir reconfigure::destination() {
    return QDir(environment().homePath());
}

}
