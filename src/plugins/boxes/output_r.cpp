/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
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
    Input(outputFileNameVariable).help("Name of the R variable holding the file name of the simulation output").equals("output_file_name");
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
    // Create a page if none are present
    _pages = Path("./*<PageR>", this).resolveMany<PageR>();
    if (_pages.empty()) {
        Box *page = MegaFactory::create<>("PageR", "", this);
        page->amend();
    }
    // Create text output if not present
    if ( Path("./*<OutputText>", this).resolveMany<Box>().empty() ) {
        Box *textOutput = MegaFactory::create<>("OutputText", "", this);
        textOutput->port("skipInitialRows")->equals(skipSteps);
        textOutput->port("useLocalDecimalChar")->equals(useLocalDecimalChar);
        textOutput->amend();
    }
}

void OutputR::initialize() {
    // Find pages in this output
    _pages = findMany<PageR>("./*<PageR>");
    numPages = _pages.size();
    // Check that only one OutputR objects exists
    QVector<OutputR*> outputs = findMany<OutputR>("*");
    if (outputs.size() > 1)
        ThrowException("Only one OutputR box is allowed").
                hint("Put all PageR boxes inside the same OutputR box").
                context(this);
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
    bool skipDefaultPage = (_pages.size() > 1);
    for (PageR *page : _pages) {
        if (skipDefaultPage && page->objectName()=="default")
            continue;
        s += "  " + page->functionName() + "(df)\n";
    }
    s += "}\n";

    s += "\nfigures <- function(df) {\n  Pages = list(";
    bool first = true;
    for (PageR *page : _pages) {
        if (skipDefaultPage && page->objectName()=="default")
            continue;
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
    openFile();
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
    QString output = makeClipboardOutput();
    environment().copyToClipboard(output);
}

QString OutputR::makeClipboardOutput() {
    QString s;
    s += "keepPages = " + convert<QString>(keepPages) + "; ";
    s += "keepVariables = " + convert<QString>(keepVariables) + "; ";
    s += "box_script_folder = \"" + environment().currentBoxScriptFolder().absolutePath() +  "\"\n";
    s += "source(\"" + environment().inputFileNamePath(begin) + "\")\n";
    s += "source(\""+_filePathR+"\")\n";
    s += outputFileNameVariable + " = \"" + environment().outputFilePath(".txt") + "\"\n";
    if (!_RCodes.isEmpty()) s += _RCodes.join("\n") + "\n";
    s += endScripts().join("");
    return s;
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
