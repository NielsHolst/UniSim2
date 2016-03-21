#include <QTextStream>
#include <base/path.h>
#include <base/publish.h>
#include "layout_r.h"
#include "plot_r.h"

using namespace base;

namespace boxes {

PUBLISH(PlotR)

PlotR::PlotR(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(layout).equals("merged");
    Input(ncol).equals(1);
    Input(nrow).equals(-1);
}

void PlotR::amend() {
    collectPorts();
}

void PlotR::collectPorts() {
    Box *page = dynamic_cast<Box*>(parent());
    Q_ASSERT(page);
    QString pageName = page->objectName(),
            plotName = objectName();
    // Loop through all tracked ports and pick those with matching page and plot name
    for (Port *port : Port::trackedPorts()) {
        bool showInThisPlot = (port->page() == pageName && port->plot() == plotName);
        if (showInThisPlot)
            _ports << port;
    }
}

void PlotR::initialize() {
    // Validate
    convert<LayoutR>(layout);
}

QString PlotR::toString() {
    QString s = "Plot: " + objectName() + "\n";
    for (const Port *port : _ports)
        s += "  Port: " + port->objectName() + "\n";
    return s;
}

inline QString apostrophed(QString s) {
    return "\"" + s + "\"";
}

QString PlotR::toScript() {
    QStringList portLabels;
    QString xLabel = apostrophed(xPortLabel());
    for (Port *port : _ports) {
        // Avoid x-axis being plotted on y-axis too
        if (port->label() != xLabel)
            portLabels << apostrophed(port->label());
    }
    // Write function call
    QString string;
    QTextStream s(&string);
    s << "    "
      << "unisim_plot_" << layout << "(df, "
      << xLabel << ", "
      << "c(" << portLabels.join(", ") << ")";

    if (convert<LayoutR>(layout) == Facetted) {
        s << ", "
          << "ncol=" << dim("ncol") << ", "
          << "nrow=" << dim("nrow");
    }
    s << "),\n";
    return string;
}

QString PlotR::xPortLabel() {
    Box *output = Path("ancestors::*{OutputR}", this).resolveOne<Box>(this);
    QString xPath = output->port("xAxis")->importPath();
    Port *xPort = Path(xPath).resolveOne<Port>(this);
    return xPort->label();
}

QString PlotR::dim(QString portName) {
    int value = port(portName)->value<int>();
    return (value == -1) ? QString("NULL") : QString::number(value);
}


}
