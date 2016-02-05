#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/publish.h>
#include "set_output_folder.h"

using namespace base;

namespace command {

PUBLISH(set_output_folder)
HELP(set_output_folder, "set output folder", "shows output folder")
HELP(set_output_folder_path, "set output folder <path>", "sets output folder")

set_output_folder::set_output_folder(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(set_output_folder);
}


void set_output_folder::doExecute() {
    Environment &env(environment());
    DialogBase &dia(dialog());

    switch (_args.size()) {
    case 3:
        break;
    case 4:
        env.state.dir.output = QDir(_args.at(3));
        break;
    default:
        throw Exception("Command 'set output folder' takes at most 1 argument");
    }

    QDir dir = locateDir(env.state.dir.work, env.state.dir.output);

    QString info;
    if (QDir(env.state.dir.output).isRelative()) {
        info = "Relative path '%1' resolves to '%2";
        info = info.arg(env.state.dir.output.path()).arg(dir.absolutePath());
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
