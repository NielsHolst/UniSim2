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
    copyFolder(source(), destination());
}

QDir reconfigure::source() {
    return QDir(":/data");
}

QDir reconfigure::destination() {
    return QDir(environment().homePath());
}

}
