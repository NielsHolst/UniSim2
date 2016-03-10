#include <memory>
#include <stdio.h>
#include <QFileInfo>
#include "base/box.h"
#include "base/box_builder.h"
#include "base/box_reader_boxes.h"
#include "base/box_reader_xml.h"
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/publish.h>
#include "load.h"

using std::unique_ptr;
using namespace base;

namespace command {

PUBLISH(load)
HELP(load, "load", "loads box script")

load::load(QString name, QObject *parent)
    : Command(name, parent)
{
}


void load::doExecute() {
    Environment &env(environment());
    QString fileName;
    switch (_args.size()) {
    case 0:
        // Zero arguments when called from run command
    case 1:
        fileName = env.state.latestLoadArg;
        break;
    case 2:
        fileName = _args.at(1);
        break;
    default:
        throw Exception("Too many arguments. Write 'load <file-name>'."
                        "\nIf the file name contains spaces then enclose it in apostrophes",
                        _args.join(" "));
    }
    delete env.state.root;
    env.state.root = 0;
    readFile(fileName);
    env.state.latestLoadArg = fileName;
}

void load::readFile(QString fileName) {
    BoxReaderBase *reader{0};
    BoxBuilder builder;
    try {
        switch(fileType(fileName)) {
        case Boxes:
            reader = new BoxReaderBoxes(&builder);
            break;
        case Xml:
            reader = new BoxReaderXml(&builder);
            break;
        }
        reader->parse(filePath(fileName));
    }
    catch (Exception &ex) {
        dialog().error(QString("Load failed\n") + ex.fullText());
    }

    Box *newRoot = builder.content();
    if (newRoot) {
        environment().state.root = newRoot;
        QString info("%1 boxes loaded");
        dialog().information(info.arg(newRoot->count()));
    }
    else
        dialog().error("Box is empty");
    delete reader;
}

load::FileType load::fileType(QString fileName) {
    QString suffix = QFileInfo(fileName).suffix().toLower();
    if (suffix == "box")
        return Boxes;
    else if (suffix == "xml")
        return Xml;
    QString s = suffix.isEmpty() ? "Missing" : "Wrong";
    throw Exception(s + " file type. Must be '.box' or '.xml'", fileName);
}

QString load::filePath(QString fileName) {
    QDir dir = locateDir(environment().state.dir.work, environment().state.dir.input);
    if (!dir.exists())
        throw Exception("Input folder not found", dir.absolutePath());
    return dir.absolutePath() + "/" + fileName;
}

}
