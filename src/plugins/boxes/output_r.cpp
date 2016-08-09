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
    Input(clear).equals(false).help("Clear R graphics and work space?");
    Input(showPlots).equals(true).help("Show R plots?");
    Input(script).help("Name of R script to run after auto-generated R script");
    Input(useRStudio).equals(true).help("Use RStudio (true) or RGui (false)");
}

void OutputR::amend() {
    // Create a page if none are present
    _pages = Path("./*{PageR}", this).resolveMany<PageR>();
    if (_pages.empty()) {
        Box *page = MegaFactory::create<Box>("PageR", "", this);
        page->amend();
    }
}

void OutputR::initialize() {
    // Find pages in this output
    _pages = Path("./*{PageR}", this).resolveMany<PageR>();
}

QString OutputR::toString() {
    QString s;
    for (PageR *page : _pages)
        s += page->toString();
    return s;
}

QString OutputR::toScript() {
    QString s;
    if (clear) {
        s += "rm(list=ls(all=TRUE))\n";
        s += "graphics.off()\n";
    }

    s += "source(\"" + environment().filePath(Environment::Script, "common.R") + "\")\n\n";
    for (PageR *page : _pages)
        s += page->toScript();
    s += "unisim_plot_all <- function(df) {\n";
    bool skipDefaultPage = (_pages.size() > 1);
    for (PageR *page : _pages) {
        if (skipDefaultPage && page->objectName()=="default")
            continue;
        s += "  " + page->functionName() + "(df)\n";
    }
    s += "}\n";
    return s;
}

//
// debrief
//

void OutputR::debrief() {
    writeScript();
    dialog().information("R script written to '" + environment().latestOutputFilePath("R") + "'");
    dialog().information("Executable R script copied to clipboard");
}

void OutputR::writeScript() {
    openFile();
    QTextStream script(&_file);
    script << toScript();
    _file.close();
}

void OutputR::openFile() {
    QString filePath = environment().outputFilePath(".R");
    filePath.replace("\\", "/");
    _file.setFileName(filePath);
    if ( !_file.open(QIODevice::WriteOnly | QIODevice::Text) )
        ThrowException("Cannot open file for output").value(filePath).context(this);
    QString s;
    s += "source(\""+filePath+"\")\n";
    s += "sim = read_unisim_output(\"" + environment().outputFilePath(".txt") + "\")\n";
    if (showPlots)
        s += "unisim_plot_all(sim)\n";
    if (!useRStudio)
        s += "bringToTop(-1)\n";
    if (!script.isEmpty())
        s += "source(\"" + environment().filePath(Environment::Script, script) + "\")\n";
    environment().copyToClipboard(s);
}

}
