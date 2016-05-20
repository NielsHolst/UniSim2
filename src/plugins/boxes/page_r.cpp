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

PageR::PageR(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(xAxis).imports("/*[step]");
    Input(width).equals(14);
    Input(height).equals(10);
    Input(ncol).equals(-1);
    Input(nrow).equals(-1);
}

void PageR::amend() {
    // Create a plot if none are present
    _plots = Path("./*{PlotR}", this).resolveMany<PlotR>();
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
    importPorts[0]->page("default");
}

void PageR::initialize() {
    // Find plots on this page
    _plots = Path("./*{PlotR}", this).resolveMany<PlotR>();
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
    s << functionName() << " <- function(df) {\n"
      << "  windows("
      << port("width")->value<int>()
      << ", "
      << port("height")->value<int>()
      << ")\n"
      << "  grid.arrange(\n" ;
    bool skipDefaultPlot = (_plots.size() > 1);
    for (PlotR *plot : _plots) {
        if (skipDefaultPlot && plot->objectName() == "default")
            continue;
        s << plot->toScript();
    }
    s << "    nrow = " << dim("nrow") << ",\n"
      << "    ncol = " << dim("ncol") << "\n  )\n}\n";
    return string;
}

QString PageR::functionName() const {
    return "unisim_" + objectName() + "_page";
}

}
