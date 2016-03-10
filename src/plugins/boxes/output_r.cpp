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
#include "layout_r.h"
#include "output_r.h"
#include "page_r.h"
#include "plot_r.h"

using namespace base;

namespace boxes {

PUBLISH(OutputR)

OutputR::OutputR(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(OutputR);
    Input(xAxis).imports("/*[step]");
    Input(layout).equals("merged");
    Input(width).equals(14);
    Input(height).equals(10);
    Input(clear).equals(true);
}

//
// amend
//

namespace {
    void copyPortValues(Box *destination, Box *source, QStringList excluding = QStringList()) {
        QVector<Port*> sourcePorts = Path(".[*]", source).resolve<Port>();
        for (Port *sourcePort : sourcePorts) {
            QString portName = sourcePort->objectName();
            if (excluding.contains(portName))
                continue;
            Port *destinationPort = destination->peakPort(portName);
            if (destinationPort) {
                if (destinationPort->hasImport())
                    destinationPort->imports(sourcePort->importPath());
                else
                    destinationPort->equals(sourcePort->value<QString>());
            }
        }
    }
}

void OutputR::amend() {
    // Collect names of pages and plots attributed to tracked ports
    QVector<Port*> trackedPorts = Port::trackedPorts();
    QMap<QString, QSet<QString>> pagesWithPlots;
    for (Port *port : trackedPorts) {
        pagesWithPlots[port->page()] << port->plot();
    }
    // Create pages with plots
    //!! Only create pages that do not exist
    //!! For pages that exist only create plots that do not exist
    QMapIterator<QString, QSet<QString>> it(pagesWithPlots);
    while (it.hasNext()) {
        it.next();
        QString pageName = it.key();
        QSet<QString> plotNames = it.value();
        Box *page = MegaFactory::create<PageR>("PageR", pageName, this);
        // The page imports the same axis as the general one for OutputR
        QStringList dontCopy;
        dontCopy << "ncol" << "nrow";
        copyPortValues(page, this, dontCopy);
//        page->port("xAxis")->imports( port("xAxis")->importPath() );
        for (QString plotName : plotNames) {
            PlotR *plot = MegaFactory::create<PlotR>("PlotR", plotName, page);
            copyPortValues(plot, page, dontCopy);
        }
    }
    // Make certain that x-axis is included in output text file
    setTrackX();
}

void OutputR::setTrackX() {
    Port *xPort = port("xAxis");
    xPort->resolveImports();
    QVector<Port*> importPorts = xPort->importPorts();
    if (importPorts.size() != 1) {
        QString msg{"Expected one x-axis, got '%1'"};
        throw Exception(msg.arg(importPorts.size()), port("xAxis")->importPath(), this);
    }
    importPorts[0]->page("");
}

//
// initialize
//

void OutputR::initialize() {
    // Validate input
    convert<LayoutR>(layout);
    // Find pages in this output
    _pages = Path("./*{PageR}", this).resolve<PageR>();
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
        throw Exception("Cannot open file for output", filePath, this);
    environment().copyToClipboard(
                "source(\""+filePath+"\")\n"
                "unisim_plot_all(read_unisim_output(\"" +
                environment().outputFilePath(".txt") +
                + "\"))\n"
                );
}

}
