/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QMap>
#include <QMapIterator>
#include <QSet>
#include <QTextStream>
#include <QVector>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/mega_factory.h>
#include <base/path.h>
#include <base/port.h>
#include <base/publish.h>
#include "output_r.h"
#include "page_r.h"
#include "plot_r.h"

using namespace base;

namespace boxes {

PUBLISH(OutputR)

OutputR::OutputR(QString name, QObject *parent)
    : Box(name, parent)
{
    help("creates output and scripts for R");
    sideEffects("writes an R script to the output folder\ncopies an R script to the clipboard");
    Input(begin).equals("scripts/begin.R").help("Name of R script run before auto-generated R script");
    Input(end).equals("scripts/end.R").help("Name of R script(s) run after auto-generated R script");
    Input(graphicsFormat).equals("none").help("Format of graphics output file; intended for use in your own end script");
    Input(destinationFolder).help("If this path to a folder is set, all outputs will be copied there");
    Input(textOutputFilePath).imports("./*[filePath]").help("Name of text output file, including absolute path");
    Input(keepPages).equals(false).help("Keep previous pages in R?");
    Input(keepVariables).equals(false).help("Keep previous variables in R?");
    Input(useLocalDecimalChar).equals(false).help("Use local decimal character in output?");
    Input(skipSteps).help("Number of steps to skip in the output");
    Input(popUp).equals(false).help("Show pages in pop-up windows?");
    Input(width).equals(7).help("Width of pop-up windows (only used if popUp is set)");
    Input(height).equals(7).help("Height of pop-up windows (only used if popUp is set)");
    Output(numPages).help("Number of pages in this output");
}

void OutputR::amend() {
//    if (_pages.empty()) {
//        Box *page = MegaFactory::create<>("PageR", "", this);
//        page->amend();
//    }
    // Create text output if not present
    if ( Path("./*<OutputText>", this).resolveMany<Box>().empty() ) {
        Box *textOutput = MegaFactory::create<>("OutputText", "", this);
        textOutput->port("skipInitialRows")->equals(skipSteps);
        textOutput->port("useLocalDecimalChar")->equals(useLocalDecimalChar);
        textOutput->amend();
    }
}

void OutputR::initialize() {
    // Find destination folder if set
    _hasDestination = !destinationFolder.isEmpty();
    if (_hasDestination) {
        _destinationDir = QDir(destinationFolder);
        if (!_destinationDir.exists())
            ThrowException("Destination folder does not exist").value(destinationFolder).context(this);
    }
    // Check that only one OutputR objects exists
    QVector<OutputR*> outputs = findMany<OutputR>("*");
    if (outputs.size() > 1)
        ThrowException("Only one OutputR box is allowed").
                hint("Put all PageR boxes inside the same OutputR box").
                context(this);
    // Open file for R script
    openFile();
}

void OutputR::reset() {
    _pages = findMany<PageR>("./*<PageR>");
    numPages = _pages.size();
}

QString OutputR::toString() {
    QString s;
    for (PageR *page : _pages)
        s += page->toString();
    return s;
}

QString OutputR::toScript() {
    QString s;
    for (PageR *page : _pages)
        s += page->toScript();
    s += "plot_all <- function(df) {\n";
//    bool skipDefaultPage = (_pages.size() > 1);
    for (PageR *page : _pages) {
//        if (skipDefaultPage && page->objectName()=="default")
//            continue;
        s += "  " + page->functionName() + "(df)\n";
    }
    s += "}\n";

    s += "\nfigures <- function(df) {\n  Pages = list(";
    bool first = true;
    for (PageR *page : _pages) {
//        if (skipDefaultPage && page->objectName()=="default")
//            continue;
        if (!first)
            s += ", ";
        first = false;
        s += toFigureListElement(page);
    }
    s += ")\n}\n";

    return s;
}

QString OutputR::toFigureListElement(PageR *page) {
    QString s = "Page = list(";
    s += "Grob=" + page->functionName() + "(df), ";
    s += "Width=" + QString::number(page->port("width")->value<double>()) + ", ";
    s += "Height=" + QString::number(page->port("height")->value<double>());
    s += ")";
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
    _file.setFileName(_filePathR);
    if ( !_file.open(QIODevice::WriteOnly | QIODevice::Text) )
        ThrowException("Cannot open file for output").value(_filePathR).context(this);
}

void OutputR::copyToClipboard() {
    // Write Rcode to clipboard
    QString output = makeOutputRCode(true);
    environment().copyToClipboard(output);

    // Write Rcode to file as well
    if (_hasDestination) {
        output = makeOutputRCode(false);
        QString fileName = QFileInfo(environment().latestLoadArg()).baseName() + ".R",
                fileNamePath = _destinationDir.absoluteFilePath(fileName);
        QFile file(fileNamePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream str(&file);
            str << output;
            file.close();
        }
        else {
            ThrowException("Cannot open destination file").value(fileNamePath).context(this);
        }
    }
}

QString OutputR::makeOutputRCode(bool forClipboard) {
    // Set up names for R and txt output files
    QString ofp = textOutputFilePath, //environment().outputFilePath(".txt"),
            outputFileName = forClipboard ? ofp : QFileInfo(ofp).fileName(),
            RFileName = forClipboard ? _filePathR : QFileInfo(_filePathR).fileName();
    // Copy output files to destination folder if set
    if (_hasDestination) {
        copyFileToDestination(ofp);
        if (numPages>0) copyFileToDestination(_filePathR);
    }
    // Put together R code
    QString s;
    s += "setwd(\"" + _destinationDir.absolutePath() + "\")\n";
    s += "keepPages = " + convert<QString>(keepPages) + "; ";
    s += "keepVariables = " + convert<QString>(keepVariables) + "; ";
    s += "box_script_folder = \"" + environment().currentBoxScriptFolder().absolutePath() +  "\"\n";
    s += "source(\"" + environment().inputFileNamePath(begin) + "\")\n";
    if (numPages>0) s += "source(\"" + RFileName + "\")\n";
    s += "output_file_name = \"" + outputFileName + "\"\n";
    s += "graphicsFormat = \"" + graphicsFormat + "\"; ";
    s += "reuseData = FALSE\n";
    if (!_RCodes.isEmpty()) s += _RCodes.join("\n") + "\n";
    s += endScripts().join("");
    return s;
}

void OutputR::copyFileToDestination(QString sourceFilePath) {
    QString fileName = QFileInfo(sourceFilePath).fileName(),
            destFilePath = _destinationDir.absolutePath() + "/" + fileName;
    if (QFile::exists(destFilePath))
        QFile::remove(destFilePath);
    bool ok = QFile::copy(sourceFilePath, destFilePath);
    if (!ok) {
        ThrowException("Could not copy file to destination").
            value(sourceFilePath + " -> " + destFilePath);
    }
}

QStringList OutputR::endScripts() {
    QStringList scripts;
    for (QString script : end) {
        QString s = "source(\"" + environment().inputFileNamePath(script) + "\")\n";
        scripts << s;
    }
    return scripts;
}

}
