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
    : OutputPorts(name, parent)
{
    help("produces an R plot");
    Input(hide).equals(false);
    Input(layout).equals("facetted").help("Either \"merged\" or \"facetted\"");
    Input(end).help("Name of R script code that will be added to the ggplot");
    Input(endCode).help("R code that will be added to the ggplot");
    Input(ncol).equals(-1);
    Input(nrow).equals(-1);
    Input(iteration).imports("/*[iteration]");
    Input(iterationId).equals("iteration");
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
    for (const Port *port : trackedPorts())
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
    if (hide || trackedPorts().isEmpty())
        return QString();
    QStringList portLabels;
    QString xLabel = apostrophed(xPortLabel());
    for (Port *port : trackedPorts()) {
        // Avoid x-axis being plotted on y-axis too
        if (port->label() != xLabel)
            portLabels << apostrophed(port->labelList());
    }
    // Write function call
    QString string;
    QTextStream s(&string);
    s << "    "
      << "plot_" << layout << "(df, "
      << xLabel << ", "
      << (iteration > 2 ? ("\""+iterationId+"\"") : "NULL") << ", "
      << "c(" << portLabels.join(", ") << ")"
      << ", "
      << "ncol=" << dim("ncol") << ", "
      << "nrow=" << dim("nrow")
      << ")";
    if (!end.isEmpty())
        s << "+" << environment().inputFileContent(end).trimmed();
    if (!endCode.isEmpty())
        s << "+" << endCode;
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
