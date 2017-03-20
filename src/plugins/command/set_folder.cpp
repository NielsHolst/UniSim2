#include <QApplication>
#include <base/command_help.h>
#include <base/publish.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include "reconfigure.h"
#include "set_folder.h"

using namespace base;

namespace command {

PUBLISH(set_folder)
HELP(set_folder, "set folder", "shows paths for all folder types")
HELP(set_folder_type, "set folder <folder type>", "shows path for this folder type")
HELP(set_folder_type_path, "set folder <folder type> <path>", "changes the path for this folder type")

set_folder::set_folder(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(set_folder);
}

void set_folder::doExecute() {
    switch (_args.size()) {
    case 2:
        showAllFolders();
        break;
    case 3:
        showFolder(_args.at(2));
        break;
    case 4:
        setFolder(_args.at(2), _args.at(3));
        break;
    default:
        ThrowException("Too many arguments").value(_args.join(" "));
        return;
    }
}

void set_folder::showAllFolders() {
    QString info =
        "Work folder:\n  " +
        environment().folderInfo(Environment::Work) +
        "\nInput folder:\n  " +
        environment().folderInfo(Environment::Input) +
        "\nOutput folder:\n  " +
        environment().folderInfo(Environment::Output) +
        "\nAtom editor folder:\n  " +
        environment().folderInfo(Environment::Atom);
        if (environment().isWindows()) {
            "\nNotepad++ editor folder:\n  " +
            environment().folderInfo(Environment::Notepad);
        }
    dialog().information(info);
}

void set_folder::showFolder(QString folderType) {
    dialog().information( environment().folderInfo(convert<Environment::Folder>(folderType)) );
}

void set_folder::setFolder(QString folderType, QString folderPath) {
    Environment::Folder folder = convert<Environment::Folder>(folderType);
    if (folderPath == "HOME")
        folderPath = reconfigure::destination().absolutePath();
    else if (folderPath == "DEV")
        folderPath = QApplication::applicationDirPath() + "/..";
    environment().dir(folder, folderPath);
    showFolder(folderType);
}

}
