/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <string>
#include <sstream>
#include <QFileInfo>
#include <base/boxscript_parser.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "load3.h"

using std::unique_ptr;
using namespace base;

namespace command {

PUBLISH(load3)
HELP(load3, "load3", "loads a box script (new syntax)")

load3::load3(QString name, QObject *parent)
    : Command(name, parent)
{
}

void load3::doExecute() {
    Environment &env(environment());
    QString fileName;
    switch (_args.size()) {
    case 1:
          fileName = env.latestLoadArg();
        break;
    case 2:
        fileName = _args.at(1);
        break;
    default:
        ThrowException("Too many arguments. Write 'load3 <file-name>'."
                        "\nIf the file name contains spaces then enclose it in apostrophes").value(_args.join(" "));
    }
    env.currentLoadArg(fileName);
    readFile(fileName);
    env.latestLoadArg(fileName);
}

void load3::readFile(QString fileName) {
    std::string
            source = environment().inputFileContent(fileName).toStdString(),
            sourcePath = environment().inputFileNamePath(fileName).toStdString();
    auto result = boxscript::parser::parse(source, sourcePath);
    std::stringstream str;
    str << *result;
    dialog().information("Boxscript read by Spirit X3:\n" + QString::fromStdString(str.str()));
//    reader->parse(environment().filePath(Environment::Input, fileName));
//    environment().computationStep(ComputationStep::Amend);
//    environment().root(builder.content());
//    dialog().information(QString("%1 boxes created").arg(environment().root()->count()));
}


}
