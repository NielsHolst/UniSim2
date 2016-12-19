#include <QFile>
#include <QTextStream>
#include "base/box.h"
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/publish.h>
#include "write.h"

using std::unique_ptr;
using namespace base;

namespace command {

PUBLISH(write)
HELP(write, "writes", "writes box script")

write::write(QString name, QObject *parent)
    : Command(name, parent)
{
}


void write::doExecute() {
    switch (_args.size()) {
    case 1:
        writeFile();
        break;
    default:
        ThrowException("'write' command takes no arguments.");
    }
}

void write::writeFile() {
    Box *root =  environment().root();
    if (!root)
        dialog().error("No box script loaded");
    else {
        QFile file;
        environment().openOutputFile(file, ".box");
        QTextStream text(&file);

        root->toText(text, WriteOptions());

        QString info("Box script written to '%1'");
        dialog().information(info.arg(environment().outputFilePath(".box")));
        environment().incrementFileCounter();
    }
}

}
