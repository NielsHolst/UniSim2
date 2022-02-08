/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QDir>
#include <QMap>
#include <QMapIterator>
#include <QSet>
#include <QTextStream>
#include <QVector>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/mega_factory.h>
#include <base/path.h>
#include <base/port.h>
#include <base/publish.h>
#include "output_r.h"
//#include "page_r.h"

using namespace base;

namespace boxes {

PUBLISH(OutputR)

OutputR::OutputR(QString name, Box *parent)
    : Box(name, parent)
{
    help("creates output and scripts for R");
    sideEffects("writes an R script to the output folder\ncopies an R script to the clipboard");
    Input(begin).equals("scripts/begin.R").help("Name of R script run before auto-generated R script");
    Input(end).equals("scripts/end.R").help("Name of R script(s) run after auto-generated R script");
    Input(keepPages).equals(false).help("Keep previous pages in R?");
    Input(keepVariables).equals(false).help("Keep previous variables in R?");
    Input(saveAsDataFrame).equals(false).help("Save output as R data frame too?");
    Input(skipSteps).help("Number of steps to skip in the output");
    Input(popUp).equals(false).help("Show pages in pop-up windows?");
    Input(width).equals(7).help("Width of pop-up windows (only used if popUp is set)");
    Input(height).equals(7).help("Height of pop-up windows (only used if popUp is set)");
    Output(numPages).help("Number of pages in this output");
}

void OutputR::initialize() {
    // Check that only one OutputR objects exists
    QVector<OutputR*> outputs = findMany<OutputR*>("*");
    if (outputs.size() > 1)
        ThrowException("Only one OutputR box is allowed").
                hint("Put all PageR boxes inside the same OutputR box").
                context(this);
    // Open file for R script
    openFile();
    // Getting the output path for .txt output
    _filePathTxt = environment().outputFilePath(".txt");
    _filePathTxt.replace("\\", "/");
}

void OutputR::reset() {
//    _pages = findMany<PageR*>("./*<PageR>");
//    numPages = _pages.size();
}

//QString OutputR::toString() {
//    QString s;
//    for (PageR *page : _pages)
//        s += page->toString();
//    return s;
//}

QString OutputR::toScript() {
    QString s;
//    for (PageR *page : _pages)
//        s += page->toScript();
//    s += "plot_all <- function(df) {\n";
//    for (PageR *page : _pages) {
//        s += "  " + page->functionName() + "(df)\n";
//    }
//    s += "}\n";

//    s += "\nfigures <- function(df) {\n  Pages = list(";
//    bool first = true;
//    for (PageR *page : _pages) {
//        if (!first)
//            s += ", ";
//        first = false;
//        s += toFigureListElement(page);
//    }
//    s += ")\n}\n";

//    if (!_RCodes.isEmpty()) s += _RCodes.join("\n") + "\n";

    return s;
}

QString OutputR::toFigureListElement(PageR */*page*/) {
    QString s = "Page = list(";
//    s += "Grob=" + page->functionName() + "(df), ";
//    s += "Width=" + QString::number(page->port("width")->value<double>()) + ", ";
//    s += "Height=" + QString::number(page->port("height")->value<double>());
//    s += ")";
    return s;

}

void OutputR::addRCode(QString s) {
    _RCodes << s;
}

//
// debrief
//

void OutputR::debrief() {
    writeScript();
    dialog().information("R script written to '" + environment().latestOutputFilePath("R") + "'");
    copyToClipboard();
}

void OutputR::writeScript() {
    QTextStream script(&_file);
    script << toScript();
    _file.close();
}

void OutputR::openFile() {
    _filePathR = environment().outputFilePath(".R");
    _filePathR.replace("\\", "/");
    if (_file.isOpen())
        _file.close();
    _file.setFileName(_filePathR);
    if ( !_file.open(QIODevice::WriteOnly | QIODevice::Text) )
        ThrowException("Cannot open file for output").value(_filePathR).context(this);
}

void OutputR::copyToClipboard() {
    QString output = makeOutputRCode();
    environment().copyToClipboard(output);
}

QString OutputR::makeOutputRCode() {
    // Put together R code
    QString s;
    s += "saveAsDataFrame = " + convert<QString>(saveAsDataFrame) + "; ";
    s += "keepPages = " + convert<QString>(keepPages) + "; ";
    s += "keepVariables = " + convert<QString>(keepVariables) + "\n";
    s += "box_script_folder = \"" + environment().currentBoxScriptFolder().absolutePath() +  "\"\n";
    s += "output_file_name  = \"" + _filePathTxt  + "\"\n";
    s += "source(\"" + environment().inputFileNamePath(begin) + "\")\n";
    if (numPages>0) s += "source(\"" + _filePathR + "\")\n";
    s += endScripts().join("");
    return s;
}

//void OutputR::copyFileToDestination(QString sourceFilePath) {
//    QString fileName = QFileInfo(sourceFilePath).fileName(),
//            destFilePath = _destinationDir.absolutePath() + "/" + fileName;
//    if (QFile::exists(destFilePath))
//        QFile::remove(destFilePath);
//    bool ok = QFile::copy(sourceFilePath, destFilePath);
//    if (!ok) {
//        ThrowException("Could not copy file to destination").
//            value(sourceFilePath + " -> " + destFilePath);
//    }
//}

QStringList OutputR::endScripts() {
    QStringList scripts;
    for (QString script : end) {
        QString s = "source(\"" + environment().inputFileNamePath(script) + "\")\n";
        scripts << s;
    }
    return scripts;
}

}
