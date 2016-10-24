#include <QDesktopServices>
#include <QUrl>
#include <base/command_help.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "edit.h"

using namespace base;

namespace command {

PUBLISH(edit)
HELP(edit, "edit", "opens box script in editor")

edit::edit(QString name, QObject *parent)
    : Command(name, parent)
{
}

void edit::doExecute() {
    Environment &env(environment());
    if (_args.size() > 1)
        ThrowException("The 'edit' command takes no arguments");
    QString url = "file:///" + env.inputFileNamePath(env.latestLoadArg());
    bool ok = QDesktopServices::openUrl(QUrl(url));
    if (!ok)
        ThrowException("Could not open editor");
}

}
