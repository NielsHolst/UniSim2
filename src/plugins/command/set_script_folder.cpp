#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/publish.h>
#include "set_script_folder.h"

using namespace base;

namespace command {

PUBLISH(set_script_folder)
HELP(set_script_folder, "set script folder", "shows script folder")
HELP(set_script_folder_path, "set script folder <path>", "sets script folder")

set_script_folder::set_script_folder(QString name, QObject *parent)
    : Command(name, parent)
{
}


void set_script_folder::doExecute() {
    Environment &env(environment());
    DialogBase &dia(dialog());

    switch (_args.size()) {
    case 3:
        break;
    case 4:
        env.state.dir.script = QDir(_args.at(3));
        break;
    default:
        ThrowException("Command 'set script folder' takes at most 1 argument");
    }

    QDir dir = locateDir(env.state.dir.work, env.state.dir.script);

    QString info;
    if (QDir(env.state.dir.script).isRelative()) {
        info = "Relative path '%1' resolves to '%2";
        info = info.arg(env.state.dir.script.path()).arg(dir.absolutePath());
    }
    else {
        info = "Absolute path '%1'";
        info = dir.absolutePath();
    }
    if (!dir.exists())
        info += "\n'" + dir.absolutePath() +  "' will be created when needed";

    dia.information(info);
}

}
