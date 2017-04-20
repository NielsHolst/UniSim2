#include <QTextStream>
#include <base/environment.h>
#include <base/mega_factory.h>
#include <base/path.h>
#include <base/port_filter.h>
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
    Input(xAxis).equals("/*[step]").notReferenced();
    Input(ncol).equals(-1).help("No. of columns to arrange plots in");
    Input(nrow).equals(-1).help("No. of rows to arrange plots in");
    Input(width).imports("ancestors::*<OutputR>[width]");
    Input(height).imports("ancestors::*<OutputR>[height]");
}

void PageR::amend() {
    // Create a plot if none are present
    _plots = Path("./*<PlotR>", this).resolveMany<PlotR>();
    if (_plots.empty()) {
        Box *plot = MegaFactory::create<>("PlotR", "", this);
        plot->amend();
    }
    // Place order(s) for x-axis
    _xAxisOrders = Track::placeOrders(xAxis, this);
    if (_xAxisOrders.isEmpty())
        ThrowException("Cannot find any matching ports for xAxis").context(this);
}

void PageR::initialize() {
    // See if pop-up is in effect
    Port *popUpPort = findOne<Port>("ancestors::*<OutputR>[popUp]");
    _popUp = popUpPort->value<bool>();

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
    if (_popUp && !environment().isMac()) {
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

QVector<Track::Order> PageR::xAxisOrders() const {
    return _xAxisOrders;
}

}
