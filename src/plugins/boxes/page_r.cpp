/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QTextStream>
#include <base/environment.h>
#include <base/path.h>
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
    Input(xAxis).equals("/*[step]");
    Input(title).help("Title shown on page");
    Input(ncol).equals(-1).help("No. of columns to arrange plots in");
    Input(nrow).equals(-1).help("No. of rows to arrange plots in");
    Input(width)     .imports("..[width]");
    Input(height)    .imports("..[height]");
    Input(plotAsList).imports("..[plotAsList]");
    Input(popUp)     .imports("..[popUp]");
    Input(numPages)  .imports("..[numPages]");
}

void PageR::initialize() {
    // Find my plots
    _plots = findMany<PlotR*>("./*");


    bool showPages = (numPages>1);
    bool dimChanged = (TestNum::ne(width, 7.) || TestNum::ne(height, 7.));

    // Raise pop-up anyway?
    _doPopUp = popUp ||
               (!environment().isMac() && (showPages || dimChanged));

    // Check for list output
    if (plotAsList && _plots.size() != 1)
        ThrowException("Plots of these types cannot be combined in one page")
                .hint("Show each PlotR in its own PageR").context(this);
}

void PageR::reset() {
    _commonPageNumber = _myPageNumber = 0;
}

QString PageR::dim(QString portName) {
    int value = port(portName)->value<int>();
    return (value == -1) ? QString("NULL") : QString::number(value);
}

QString PageR::toScript() {
    QString string;
    QTextStream s(&string);
    s << functionName() << " <- function(df, ...) {\n";
    if (_doPopUp) {
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
            if (skipDefaultPlot && plot->name() == "default")
                continue;
            s << plot->toScript();
        }
    }
    if (!title.isEmpty())
        s << "    top = \"" << title << "\",\n";
    s << "    ...,\n"
      << "    nrow = " << dim("nrow") << ",\n"
      << "    ncol = " << dim("ncol") << "\n  )";
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
