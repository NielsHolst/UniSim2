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
    Class(load);
}


void load::doExecute() {
    Environment &env(environment());
    switch (_args.size()) {
    case 1:
        throw Exception("Write 'load <file-name>'");
        break;
    case 2:
        delete env.state.root;
        env.state.root = 0;
        readFile(_args.at(1));
        env.state.latestLoadArg = _args.at(1);
        break;
    default:
        throw Exception("Too many arguments. Write 'load <file-name>'."
                        "\nIf the file name contains spaces then enclose it in apostrophes");
    }
}

void load::readFile(QString fileName) {
    BoxReaderBase *reader;
    switch(fileType(fileName)) {
    case Boxes:
        reader = new BoxReaderBoxes;
        break;
    case Xml:
        reader = new BoxReaderXml;
        break;
    }

    BoxBuilder builder;
    try {
        builder = reader->parse(filePath(fileName));
    }
    catch (Exception &ex) {
        throw Exception(QString("Load failed\n") + ex.fullText());
    }
    Box *newRoot = builder.content();
    if (newRoot) {
        environment().state.root = newRoot;
        QString info("New root: %1 %2");
        dialog().information(info.arg(newRoot->className()).arg(newRoot->objectName()));
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
    throw Exception("Wrong file type. Must be 'box' or 'xml'", suffix, this);
}

QString load::filePath(QString fileName) {
    QDir dir = locateDir(environment().state.dir.work, environment().state.dir.input);
    if (!dir.exists())
        throw Exception("Input folder not found", dir.absolutePath());
    return dir.absolutePath() + "/" + fileName;
}

}
