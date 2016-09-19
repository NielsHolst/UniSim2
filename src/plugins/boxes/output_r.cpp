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
    Input(begin).equals("begin.R").help("Name of R script run before auto-generated R script");
    Input(end).equals("end.R").help("Name of R script run after auto-generated R script");
    Input(outputFileNameVariable).help("Name of the R variable holding the file name of the simulation output").equals("output_file_name");
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
    s += "source(\"" + environment().filePath(Environment::Script, begin) + "\")\n";
    s += "source(\""+_filePathR+"\")\n";
    s += outputFileNameVariable + " = \"" + environment().outputFilePath(".txt") + "\"\n";
    s += "source(\"" + environment().filePath(Environment::Script, end) + "\")\n";
    environment().copyToClipboard(s);
}

}
