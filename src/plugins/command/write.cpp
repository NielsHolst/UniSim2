#include <QDesktopServices>
#include <QFile>
#include <QUrl>
#include <QTextStream>
#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog_base.h>
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
    writeFile();
    if (_args.contains("edit"))
        edit();
}

void write::writeFile() {
    Box *root =  environment().root();
    if (!root)
        dialog().error("No box script loaded");
    else {
        QFile file;
        environment().openOutputFile(file, ".box");
        QTextStream text(&file);

        root->toText(text);
        _filePath = environment().outputFilePath(".box");

        QString info("Box script written to '%1'");
        dialog().information(info.arg(_filePath));
        environment().incrementFileCounter();
    }
}

void write::edit() {
    QString url = "file:///" + _filePath;
    bool ok = QDesktopServices::openUrl(QUrl(url));
    if (!ok)
        ThrowException("Could not open editor");
}

}
