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
    Input(hide).equals(false).help("Hide this plot?");
    Input(xAxis).equals("/*[step]").notReferenced();
    Input(ncol).equals(-1).help("No. of columns to arrange plots in");
    Input(nrow).equals(-1).help("No. of rows to arrange plots in");
    Input(title).help("Title shown on page");
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
    Box *outputR = findOne<Box>("ancestors::*<OutputR>)");
    _popUp = outputR->port("popUp")->value<bool>();
    int numPages = outputR->findMany<Box>("./*<PageR>").size();
    bool keepPages = outputR->port("keepPages")->value<bool>();

    // Raise pop-up anyway?
    if (!_popUp)
        _popUp = !environment().isMac() && (numPages>1 || keepPages);

    // Find plots on this page
    _plots = findMany<PlotR>("./*<PlotR>");
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
    if (!hide) {
        if (_popUp) {
          s << "  open_plot_window("
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
        if (!title.isEmpty())
            s << "    top = \"" << title << "\",\n";
        s << "    ...,\n"
          << "    nrow = " << dim("nrow") << ",\n"
          << "    ncol = " << dim("ncol") << "\n  )";
    }
    s << "\n}\n";
    s << PlotR::variablesScript();
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
