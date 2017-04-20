#include <QTextStream>
#include <base/environment.h>
#include <base/path.h>
#include <base/publish.h>
#include "layout_r.h"
#include "page_r.h"
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
    Input(ggplot).equals("geom_line(size=1.1)").help("R code that will be added to the ggplot");
    Input(end).help("Deprecated");
    Input(endCode).help("Deprecated");
    Input(ncol).equals(-1);
    Input(nrow).equals(-1);
    Input(iteration).imports("/*[iteration]");
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
    for (const Track *track : tracks())
        s += "  Port: " + track->uniqueName() + "\n";
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
    if (hide || tracks().isEmpty())
        return QString();

    QStringList xLabels;
    for (Track *track : xAxisTracks())
        xLabels << apostrophed(track->uniqueNameExpanded());

    QStringList portLabels;
    for (Track *track : tracks())
        portLabels << apostrophed(track->uniqueNameExpanded());

    // Set iteration label according to filter
    bool isFiltered = (xAxisTracks().at(0)->filter() != PortFilter::None);
    QString iterationLabel = "iteration";
    if (isFiltered) iterationLabel += ".end";

    // Write function call
    QString string;
    QTextStream s(&string);
    s << "    "
      << "plot_" << layout << "(df, "
      << "c(" << xLabels.join(", ") << ")"
      << ", "
      << (iteration > 2 ? ("\""+iterationLabel+"\"") : "NULL")
      << ", "
      << "c(" << portLabels.join(", ") << ")"
      << ", "
      << "ncol=" << dim("ncol")
      << ", "
      << "nrow=" << dim("nrow")
      << ")";
    if (!ggplot.isEmpty())
        s << "+" << ggplot;
    if (!end.isEmpty())
        s << "+" << environment().inputFileContent(end).trimmed();
    if (!endCode.isEmpty())
        s << "+" << endCode;
    s << ",\n";
    return string;
}

QVector<Track*> PlotR::xAxisTracks() {
    QVector<Track*> tracks;
    PageR *page = Path("ancestors::*<PageR>", this).resolveOne<PageR>(this);
    QVector<Track::Order> orders = page->xAxisOrders();
    for (Track::Order order : orders)
        tracks << Track::find(order);
    return tracks;
}

QString PlotR::dim(QString portName) {
    int value = port(portName)->value<int>();
    return (value == -1) ? QString("NULL") : QString::number(value);
}


}
