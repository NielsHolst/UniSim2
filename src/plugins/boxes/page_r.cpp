#include <QTextStream>
#include <base/mega_factory.h>
#include <base/path.h>
#include <base/publish.h>
#include "layout_r.h"
#include "page_r.h"
#include "plot_r.h"

using namespace base;

namespace boxes {

PUBLISH(PageR)

int PageR::_commonPageNumber;

PageR::PageR(QString name, QObject *parent)
    : Box(name, parent)
{
    help("produces a page of plots for R");
    Input(xAxis).imports("/*[step]");
    Input(ncol).equals(-1).help("No. of columns to arrange plots in");
    Input(nrow).equals(-1).help("No. of rows to arrange plots in");
    Input(popUp).imports("ancestors::*[popUp]");
    Input(width).imports("ancestors::*[width]");
    Input(height).imports("ancestors::*[height]");
}

void PageR::amend() {
    // Create a plot if none are present
    _plots = Path("./*<PlotR>", this).resolveMany<PlotR>();
    if (_plots.empty()) {
        Box *plot = MegaFactory::create<Box>("PlotR", "", this);
        plot->amend();
    }
    // Make certain xAxis port is tracked
    Port *xPort = port("xAxis");
    xPort->resolveImports();
    QVector<Port*> importPorts = xPort->importPorts();
    if (importPorts.size() != 1) {
        QString msg{"Expected one x-axis, got '%1'"};
        ThrowException(msg.arg(importPorts.size())).value(xPort->importPath()).context(this);
    }
    importPorts[0]->track();
}

void PageR::initialize() {
    // Find plots on this page
    _plots = Path("./*<PlotR>", this).resolveMany<PlotR>();
}
void PageR::reset() {
    _commonPageNumber = _myPageNumber = 0;
}

QString PageR::toString() {
    QString s = className() + " " + objectName() + "\n";
    for (PlotR *plot : _plots)
        s += plot->toString();
    return s;
}

QString PageR::dim(QString portName) {
    int value = port(portName)->value<int>();
    return (value == -1) ? QString("NULL") : QString::number(value);
}


QString PageR::toScript() {
    QString string;
    QTextStream s(&string);
    s << functionName() << " <- function(df, ...) {\n";
    if (popUp) {
      s << "  open_graph("
        << port("width")->value<int>()
        << ", "
        << port("height")->value<int>()
        << ")\n";
    }
    s << "  grid.arrange(\n" ;
    bool skipDefaultPlot = (_plots.size() > 1);
    for (PlotR *plot : _plots) {
        if (skipDefaultPlot && plot->objectName() == "default")
            continue;
        s << plot->toScript();
    }
    s << "    ...,\n"
      << "    nrow = " << dim("nrow") << ",\n"
      << "    ncol = " << dim("ncol") << "\n  )\n}\n";
    return string;
}

QString PageR::functionName() {
    if (objectName().isEmpty() && _myPageNumber == 0)
        _myPageNumber = ++_commonPageNumber;

    QString s = "page_";
    s += objectName().isEmpty() ? QString::number(_myPageNumber) : objectName();
    return s;
}

}
