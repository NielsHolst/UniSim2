/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QTextStream>
#include <base/environment.h>
#include <base/mega_factory.h>
#include <base/path.h>
#include <base/port.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "layout_r.h"
#include "page_r.h"
#include "plot_r.h"

using namespace base;

namespace boxes {

PUBLISH(PageR)

int PageR::_commonPageNumber;

PageR::PageR(QString name, Box *parent)
    : Box(name, parent)
{
    help("produces a page of plots for R");
    Input(xAxis).imports("/*[step]");
    Input(show).equals(true).help("Show this page?");
    Input(ncol).equals(-1).help("No. of columns to arrange plots in");
    Input(nrow).equals(-1).help("No. of rows to arrange plots in");
    Input(title).help("Title shown on page");
    Input(width).imports("ancestors::*<OutputR>[width]");
    Input(height).imports("ancestors::*<OutputR>[height]");
    Input(plotAsList).computes("any(./*[plotAsList])").help("Any plot produced as a list of plots?");
}

void PageR::amend() {
    // Create a plot if none are present
    _plots = Path("./*<PlotR>", this).findMany<PlotR*>();
    if (_plots.empty()) {
        Box *plot = MegaFactory::create<>("PlotR", "", this);
        plot->amend();
    }
}

void PageR::initialize() {
    // See if pop-up is in effect
    Box *outputR = findOne<Box*>("ancestors::*<OutputR>)");
    _popUp = outputR->port("popUp")->value<bool>();
    int numPages = outputR->findMany<Box*>("./*<PageR>").size();
    bool keepPages = outputR->port("keepPages")->value<bool>();
    bool showPages = (numPages>1 || keepPages);
    bool dimChanged = (TestNum::ne(width, 7.) || TestNum::ne(height, 7.));

    // Raise pop-up anyway?
    if (!_popUp)
        _popUp = !environment().isMac() && (showPages || dimChanged);

    // Find plots on this page
    _plots = findMany<PlotR*>("./*<PlotR>");

    // Check for list output
    if (plotAsList && _plots.size() != 1)
        ThrowException("Plots of these types cannot be combined in one page")
                .hint("Show each PlotR in its own PageR").context(this);
}

void PageR::reset() {
    _commonPageNumber = _myPageNumber = 0;
}

//QString PageR::toString() {
//    QString s = className() + " " + objectName() + "\n";
//    for (PlotR *plot : _plots)
//        s += plot->toString();
//    return s;
//}

QString PageR::dim(QString portName) {
    int value = port(portName)->value<int>();
    return (value == -1) ? QString("NULL") : QString::number(value);
}

QString PageR::toScript() {
    QString string;
    QTextStream s(&string);
    s << functionName() << " <- function(df, ...) {\n";
    if (show) {
        if (_popUp) {
          s << "  open_plot_window("
            << port("width")->value<int>()
            << ", "
            << port("height")->value<int>()
            << ")\n";
        }
        if (plotAsList) {
            s << "  grid.arrange(\ngrobs=    "
              << _plots.at(0)->toScript();
        }
        else {
            s << "  grid.arrange(\n" ;
            bool skipDefaultPlot = (_plots.size() > 1);
            for (PlotR *plot : _plots) {
                if (skipDefaultPlot && plot->objectName() == "default")
                    continue;
                s << plot->toScript();
            }
        }
        if (!title.isEmpty())
            s << "    top = \"" << title << "\",\n";
        s << "    ...,\n"
          << "    nrow = " << dim("nrow") << ",\n"
          << "    ncol = " << dim("ncol") << "\n  )";
    }
    s << "\n}\n";
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
