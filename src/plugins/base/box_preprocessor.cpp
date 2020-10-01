/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <string>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMapIterator>
#include <QSet>
#include <QString>
#include "box_preprocessor.h"
#include "dialog.h"
#include "exception.h"
#include "factory_plug_in.h"
#include "mega_factory.h"

using namespace std;

namespace base {

BoxPreprocessor::BoxPreprocessor()
{
}

QString BoxPreprocessor::preprocess(QString filePath) {
    // Read box script and expand macros
    QString code = readFile(filePath, FileIncludes());
    // Set using directive and comment it out
    setUsing(extractUsing(code));
    code = code.replace("^#using", "// #using");
    // Replace any macros
    return replaceMacros(code);
}

QString BoxPreprocessor::readFile(QString filePath, FileIncludes earlierIncludes) {
    if (earlierIncludes.isEmpty())
        _origFilePath = filePath;
    _curFilePath = filePath;
    QFile file;
    file.setFileName(filePath);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QString msg("Could not open file for reading");
        ThrowException(msg).value(filePath);
    }
    // Make certain code ends with a line break
    QString code = QString(file.readAll());
    if (!code.endsWith("\n"))
        code += "\n";
    // Expand any includes
    return expandIncludes(filePath, code, earlierIncludes);
}

QString BoxPreprocessor::expandIncludes(QString sourceFilePath, QString code, FileIncludes earlierIncludes) {
    Positions includes = findDirective(code, "include");
    QString newCode;
    int at = 0;
    for (Position include : includes) {
        newCode += code.mid(at, include.begin-1);
        at = include.end;
        QString line = code.mid(include.begin, include.end - include.begin),
                includeFilePath = extractIncludeFilePath(line);
        FileInclude newFileInclude = FileInclude{sourceFilePath, includeFilePath};
//        dialog().information(sourceFilePath + " includes\n" + includeFilePath);
        checkCyclicIncludes(newFileInclude, earlierIncludes);
        earlierIncludes << newFileInclude;
        QString paths = "(" + sourceFilePath + " -> " + includeFilePath + ")\n";
        newCode +=
                "\n// Begin include " + paths +
                readFile(includeFilePath, earlierIncludes) +
                "\n// End include "  + paths;
    }
    newCode += code.mid(at);
    return newCode;
}

QString BoxPreprocessor::extractUsing(QString code) {
    // Collect the using directives in one set
    QSet<QString> usingPlugin;
    Positions includes = findDirective(code, "using");
    for (Position include : includes) {
        QString line = code.mid(include.begin, include.end - include.begin);
        QStringList items = line.split(" ", QString::SkipEmptyParts);
        if (items.size() != 2)
            ThrowException("Write '#using <plugin>").value(line);
        usingPlugin << items.at(1);
    }
    // Only zero or one member is legal
    QString result;
    switch (usingPlugin.size()) {
    case 0:
        result = "";
        break;
    case 1:
        result = usingPlugin.toList().at(0);
        break;
    default:
        ThrowException("Only one #using directive is allowed").
                value(QStringList(usingPlugin.toList()).join(", "));
    }
    return result;
}

void BoxPreprocessor::setUsing(QString pluginName) {
    auto plugins = MegaFactory::factories();
    QStringList pluginNames;
    for (int i=0; i < plugins.size(); ++i) {
        pluginNames << plugins.at(i)->id();
    }
    if (pluginName.isEmpty())
        MegaFactory::usingPlugin("");
    else if (pluginNames.contains(pluginName))
        MegaFactory::usingPlugin(pluginName);
    else
        ThrowException("Plugin name in #using directive does not exist").value(pluginName).
                hint("Existing plugins:\n" + pluginNames.join("\n"));
}

BoxPreprocessor::Positions BoxPreprocessor::findDirective(QString code, QString directive) const {
    Positions positions;
    QString find = "#" + directive;
    int at = 0;
    while (true) {
        at = code.indexOf(find, at);
        if (at==-1 || isComment(code.mid(0,at)))
            break;
        int begin = at;
        at = code.indexOf("\n", at);
        positions << Position{begin, at};
    }
    return positions;
}

bool BoxPreprocessor::isComment(QString s) const {
    // Search backwards for comment slashes in this line
    bool comment = false;
    for (int i = s.size()-2; i>=0; --i) {
        if (s.at(i) == "\n")
            break;
        else if (s.mid(i,i+1) == "//") {
            comment = true;
            break;
        }
    }
    return comment;
}

QString BoxPreprocessor::extractIncludeFilePath(QString includeLine) const {
    // Retrieve file name
    int at = includeLine.indexOf("#include");
    QString relFilePath = includeLine.mid(at+8).trimmed();
    // Deal with apostrophes
    if (!(relFilePath.startsWith("\"") && relFilePath.endsWith("\""))) {
        ThrowException("File name after #include must be in apostrophes").
                value(includeLine).value2("In " + _curFilePath);
    }
    relFilePath = relFilePath.mid(1,relFilePath.size()-2);
    // Construct absolute file path
    QDir dir = QDir(QFileInfo(_origFilePath).absolutePath());
    return QFileInfo(dir, relFilePath).absoluteFilePath();
}

void BoxPreprocessor::checkCyclicIncludes(FileInclude newFileInclude, FileIncludes earlierIncludes) const {
    for (FileInclude fileInclude : earlierIncludes) {
        if (fileInclude.includeFilePath == newFileInclude.includeFilePath) {
            ThrowException("Cyclical include").value(fileInclude.sourceFilePath).
                    hint(backtrack(earlierIncludes));
        }
    }
}

QString BoxPreprocessor::backtrack(FileIncludes earlierIncludes) const {
    QString track;
    for (FileInclude fileInclude : earlierIncludes) {
        track += "\n" + fileInclude.sourceFilePath + " -> " + fileInclude.includeFilePath;
    }
    return track;
}

QString BoxPreprocessor::replaceMacros(QString code) {
    QMap<QString, QString> definitions = extractMacros(code);
    QMapIterator<QString, QString> def(definitions);
    while (def.hasNext()) {
        def.next();
        code = code.replace("$"+def.key(), def.value());
        code = code.replace("#define", "// Effectuated macro: ");
    }
    return code;
}


QMap<QString,QString> BoxPreprocessor::extractMacros(QString code) {
    Positions defines = findDirective(code, "define");
    QMap<QString, QString> definitions;
    for (Position define : defines) {
        QString assignment = code.mid(define.begin + 7, define.end - define.begin - 7);
        int splitAt = assignment.indexOf("=");
        if (splitAt == -1)
            ThrowException("#define line must contain an equal sign").value(assignment).value2("In " + _curFilePath);
        QString name  = assignment.mid(0,splitAt).trimmed(),
                value = assignment.mid(splitAt+2).trimmed();
        definitions[name] = value;
    }
    return definitions;
}

} // namespace
