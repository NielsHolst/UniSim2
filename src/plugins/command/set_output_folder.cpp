#include <base/command.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "set_output_folder.h"

using namespace base;

namespace command {

PUBLISH(set_output_folder)

set_output_folder::set_output_folder(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(set_output_folder);
}

void set_output_folder::execute() {
    Environment &env(environment());
    DialogBase &dia(dialog());

    switch (_args.size()) {
    case 3:
        break;
    case 4:
        env.state.outputFolder = _args.at(3);
        break;
    default:
        dia.error("Command 'set output folder' takes at most 1 argument");
        return;
    }

    QDir output = env.state.dir;
    bool exists = output.cd(env.state.outputFolder);
    QString info = output.absolutePath();
    if (!exists) {
        QString absPath = env.state.dir.absolutePath() + "/" + env.state.outputFolder;
        absPath = QDir::toNativeSeparators(absPath);
        info = "'" + absPath +  "' will be created when needed";
    }
    dia.information(info);
}

}
