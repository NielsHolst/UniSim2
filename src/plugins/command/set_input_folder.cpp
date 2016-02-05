#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/publish.h>
#include "set_input_folder.h"

using namespace base;

namespace command {

PUBLISH(set_input_folder)
HELP(set_input_folder, "set input folder", "shows input folder")
HELP(set_input_folder_path, "set input folder <path>", "sets input folder")

set_input_folder::set_input_folder(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(set_input_folder);
}


void set_input_folder::doExecute() {
    Environment &env(environment());
    DialogBase &dia(dialog());

    switch (_args.size()) {
    case 3:
        break;
    case 4:
        env.state.dir.input = QDir(_args.at(3));
        break;
    default:
        throw Exception("Command 'set input folder' takes at most 1 argument");
        return;
    }

    QDir dir = locateDir(env.state.dir.work, env.state.dir.input);

    QString info;
    if (QDir(env.state.dir.input).isRelative()) {
        info = "Relative path '%1' resolves to '%2";
        info = info.arg(env.state.dir.input.path()).arg(dir.absolutePath());
    }
    else {
        info = "Absolute path '%1'";
        info = dir.absolutePath();
    }
    if (!dir.exists())
        info += "\nWarning: '" + dir.absolutePath() +  "' does not exist";

    dia.information(info);
}

}
