#include <QDir>
#include <QMap>
#include <QMapIterator>
#include <QSet>
#include <QTextStream>
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
    sideEffects("writes an R script to the output folder\n"
                "copies another R script to the clipboard");
    Input(begin).equals("scripts/begin.R").help("Name of R script run before auto-generated R script");
    Input(end).equals("scripts/end.R").help("Name of R script run after auto-generated R script");
    Input(outputFileNameVariable).help("Name of the R variable holding the file name of the simulation output").equals("output_file_name");
    Input(keepPlots).equals(false).help("Keep previous plots in R?");
    Input(keepVariables).equals(false).help("Keep previous variables in R?");
    Input(popUp).equals(false).help("Show pages in pop-up windows?");
    Input(width).equals(14).help("Width of pop-up windows (only used if popUp is set)");
    Input(height).equals(10).help("Height of pop-up windows (only used if popUp  is set)");
}

void OutputR::amend() {
    // Create a page if none are present
    _pages = Path("./*<PageR>", this).resolveMany<PageR>();
    if (_pages.empty()) {
        Box *page = MegaFactory::create<Box>("PageR", "", this);
        page->amend();
    }
    // Create text output if not present
    if ( Path("./*<OutputText>", this).resolveMany<Box>().empty() ) {
        Box *textOutput = MegaFactory::create<Box>("OutputText", "", this);
        textOutput->amend();
    }
}

void OutputR::initialize() {
    // Find pages in this output
    _pages = Path("./*<PageR>", this).resolveMany<PageR>();
}

QString OutputR::toString() {
    QString s;
    for (PageR *page : _pages)
        s += page->toString();
    return s;
}

QString OutputR::toScript() {
    QString s;
    if (popUp && !environment().isMac())
        s += popUpCode();
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
    return s;
}

QString OutputR::popUpCode() {
    return  "open_graph = function(width=7, height=7, ...) {\n"
              "if (.Platform$OS.type != \"windows\") {\n"
                "X11(width=width, height=height, type=\"cairo\", ...)\n"
              "} else {\n"
                "windows(width=width, height=height, ...)\n"
              "}\n"
            "}\n";
}

//
// debrief
//

void OutputR::debrief() {
    writeScript();
    dialog().information("R script written to '" + environment().latestOutputFilePath("R") + "'");
    copyToClipboard();
    dialog().information("Executable R script copied to clipboard");
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
    QString s;
    s += "keepPlots = " + convert<QString>(keepPlots) + "; ";
    s += "keepVariables = " + convert<QString>(keepVariables) + "\n";
    s += "source(\"" + environment().inputFileNamePath(begin) + "\")\n";
    s += "source(\""+_filePathR+"\")\n";
    s += outputFileNameVariable + " = \"" + environment().outputFilePath(".txt") + "\"\n";
    s += "source(\"" + environment().inputFileNamePath(end) + "\")\n";
    environment().copyToClipboard(s);
}

}
