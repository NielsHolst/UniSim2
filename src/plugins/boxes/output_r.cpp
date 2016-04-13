#include <QDir>
#include <QMap>
#include <QMapIterator>
#include <QSet>
#include <QTextStream>
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
    Input(ports);
    Input(clear).equals(true);
}

void OutputR::amend() {
    // Create a page if none are present
    _pages = Path("./*{PageR}", this).resolveMany<PageR>();
    if (_pages.empty()) {
        Box *page = MegaFactory::create<Box>("PageR", "", this);
        page->amend();
    }
    // Additional ports which will show in text output file but not in R plots
    addExtraPorts();
}

void OutputR::addExtraPorts() {
    // Additional ports to be trackes
    for (QString portName : ports) {
        QVector<Port*> trackedPorts = Path(portName).resolveMany<Port>();
        if (trackedPorts.isEmpty())
            ThrowException("Port not found").value(portName);
        // Add ports to blank page (which will not show)
        for (Port *port : trackedPorts) {
            if (port->page().isNull())
                port->page("");
        }
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
    s += "rm(list=ls(all=TRUE))\n";
    if (clear)
        s += "graphics.off()\n";
    s += "source(\"" + environment().scriptFilePath("common.R") + "\")\n\n";
    for (PageR *page : _pages)
        s += page->toScript();
    s += "unisim_plot_all <- function(df) {\n";
    bool skipDefaultPage = (_pages.size() > 1);
    for (PageR *page : _pages) {
        if (skipDefaultPage && page->objectName().isEmpty())
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
    environment().copyToClipboard(
                "source(\""+filePath+"\")\n"
                "unisim_plot_all(read_unisim_output(\"" +
                environment().outputFilePath(".txt") +
                + "\"))\n"
                + "bringToTop(-1)\n"
                );
}

}
