/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
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
    case 1:
          fileName = env.latestLoadArg();
//        dialog().loadWithFilePicker();
        break;
    case 2:
        fileName = _args.at(1);
        break;
    default:
        ThrowException("Too many arguments. Write 'load <file-name>'."
                        "\nIf the file name contains spaces then enclose it in apostrophes").value(_args.join(" "));
    }
    env.currentLoadArg(fileName);
    readFile(fileName);
    env.latestLoadArg(fileName);
}

void load::readFile(QString fileName) {
    BoxReaderBase *reader{nullptr};
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
        reader->parse(environment().filePath(Environment::Input, fileName));
        environment().computationStep(ComputationStep::Amend);
        environment().root(builder.content());
        dialog().information(QString("%1 boxes created").arg(environment().root()->count()));
    }
    catch (Exception &ex) {
        dialog().error(QString("Load failed\n") + ex.what());
    }
    delete reader;
}

load::FileType load::fileType(QString fileName) {
    QString suffix = QFileInfo(fileName).suffix().toLower();
    if (suffix == "box")
        return Boxes;
    else if (suffix == "xml")
        return Xml;
    QString s = suffix.isEmpty() ? "Missing" : "Wrong";
    ThrowException(s + " file type. Must be '.box' or '.xml'").value(fileName);
}

}
