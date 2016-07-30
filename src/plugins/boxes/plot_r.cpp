#include <QTextStream>
#include <base/environment.h>
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
    Input(hide).equals(false);
    Input(ports);
    Input(layout).equals("facetted").help("Either \"merged\" or \"facetted\"");
    Input(script).help("R code that will be added to the ggplot");
    Input(ncol).equals(-1);
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
    // Loop through all tracked ports and capture those with matching page and plot name
    for (Port *port : Port::trackedPorts()) {
        bool showInThisPlot = (port->page() == pageName && port->plot() == plotName);
        if (showInThisPlot)
            _ports << port;
    }
    // Additional ports listed as input for this plot
    for (QString portName : ports) {
        QVector<Port*> trackedPorts = resolveMany<Port>(portName);
        if (trackedPorts.isEmpty())
            ThrowException("Port not found").value(portName);
        // Only add ports not already captured
        for (Port *port : trackedPorts) {
            if (_ports.contains(port))
                continue;
            // Add empty string to ensure page and plot names are not Null
            port->page(page->objectName()+"");
            port->plot(objectName()+"");
            _ports << port;
        }
    }
}

void PlotR::initialize() {
    // Validate
    convert<LayoutR>(layout);
}

void PlotR::reset() {
    if (ncol==-1 && nrow==-1)
        ncol = 1;
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

inline QStringList apostrophed(QStringList list) {
    QStringList result;
    for (int i = 0; i < list.size(); ++i)
        result << apostrophed(list.at(i));
    return result;
}

QString PlotR::toScript() {
    if (hide || _ports.isEmpty())
        return QString();
    QStringList portLabels;
    QString xLabel = apostrophed(xPortLabel());
    for (Port *port : _ports) {
        // Avoid x-axis being plotted on y-axis too
        if (port->label() != xLabel)
            portLabels << apostrophed(port->labelList());
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
    s << ")";
    if (!script.isEmpty())
        s << "+" << environment().fileContent(Environment::Script, script).trimmed();
    s << ",\n";
    return string;
}

QString PlotR::xPortLabel() {
    Box *output = Path("ancestors::*{PageR}", this).resolveOne<Box>(this);
    QString xPath = output->port("xAxis")->importPath();
    Port *xPort = Path(xPath).resolveOne<Port>(this);
    return xPort->label();
}

QString PlotR::dim(QString portName) {
    int value = port(portName)->value<int>();
    return (value == -1) ? QString("NULL") : QString::number(value);
}


}
