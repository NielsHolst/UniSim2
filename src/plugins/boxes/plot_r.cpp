/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/environment.h>
#include <base/path.h>
#include <base/port.h>
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
    Input(hide).equals(false).help("Hide this plot?");
    Input(layout).equals("facetted").help("Either \"merged\" or \"facetted\"");
    Input(type).equals("default").help("Type of plot").unit("default|density|histogram(nbins)|SobolConvergence|SobolIndices");
    Input(guideTitle).help("Title of guide legends");
    Input(fontSize).help("Used for axes and panels; zero yields default font size");
    Input(ggplot).help("R code that will be added to the ggplot");
    Input(transform).help("Transformation of y-axis; only 'log10' available");
    Input(end).help("Deprecated");
    Input(endCode).help("Deprecated");
    Input(maxData).help("Max. number of data rows to plot; ignored if zero");
    Input(ncol).equals(-1).help("Number of columns in arrangement of plots; -1 keeps default");
    Input(nrow).equals(-1).help("Number of rows in arrangement of plots; -1 keeps default");
    Input(iteration).imports("/*[iteration]");
    Input(width).imports("..[width]").help("May be used to spawn additional pages");
    Input(height).imports("..[height]").help("May be used to spawn additional pages");
    Input(xAxis).imports("..[xAxis]");
    Output(plotAsList).noReset().help("Is the plot a list of plots?");
}

void PlotR::initialize() {
    // Validate
    convert<LayoutR>(layout);
    if (transform!="" && transform!="log10")
        ThrowException("Only valid value for 'transform' is 'log10'")
                .value(transform).context(this);
    // Set flag for list output
    plotAsList = (type.toLower()=="sobolindices");
}

void PlotR::reset() {
    if (ncol==-1 && nrow==-1)
        ncol = 1;
}

//QString PlotR::toString() {
//    QString s = "Plot: " + objectName() + "\n";
//    for (const Track *track : tracks())
//        s += "  Port: " + track->uniqueName() + "\n";
//    return s;
//}

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
    if (hide)
        return QString();

    QStringList xLabels;
    for (auto port : xAxis.resolved())
        xLabels << port->outputNames();

    QStringList yLabels;
    for (auto port : ports.resolved())
        yLabels << port->outputNames();

    // Write function call
    QString string,
            typeId = type.toLower();
    QTextStream s(&string);
    if (typeId=="default")
        s << scriptForDefaultPlot(xLabels, yLabels, "iteration");
    else if (typeId=="density")
        s << scriptForDensityPlot(yLabels);
    else if (typeId.startsWith("histogram"))
        s << scriptForHistogramPlot(yLabels, type);
    else if (typeId=="sobolconvergence")
        s << scriptForSobolConvergencePlot();
    else if (typeId=="sobolindices")
        s << scriptForSobolIndicesPlot();
    else
        ThrowException("Invalid value for 'type'. Must be one of " + port("type")->unit()).value(type).context(this);

    // Append code
    appendGgplot(s);
    if (!end.isEmpty())
        s << "+" << environment().inputFileContent(end).trimmed();
    if (!endCode.isEmpty())
        s << "+" << endCode;
    if (fontSize > 0 && !plotAsList )
        s << "+ggplot_theme(" << fontSize << ")";
    s << ",\n";
    return string;
}

inline QString df(int maxData) {
    return (maxData==0) ? "df" : ("df[1:" + QString::number(maxData) + ",]");
}

QString PlotR::scriptForDefaultPlot(QStringList xLabels, QStringList yLabels, QString iterationLabel) const {
    QString string;
    QTextStream s(&string);
    s << "    "
      << "plot_" << layout << "("
      << df(maxData)
      << ", "
      << "c(" << xLabels.join(", ") << ")"
      << ", "
      << (iteration > 2 ? ("\""+iterationLabel+"\"") : "NULL")
      << ", "
      << "c(" << yLabels.join(", ") << ")"
      << ", "
      << "ytrans=" << apostrophed(transform)
      << ", "
      << "ncol=" << dim("ncol")
      << ", "
      << "nrow=" << dim("nrow")
      << ")";
    return string;
}

QString PlotR::scriptForDensityPlot(QStringList yLabels) const {
    QString string;
    QTextStream s(&string);
    s << "    "
      << "plot_density("
      << df(maxData)
      << ", "
      << "c(" << yLabels.join(", ") << ")"
      << ", "
      << "ncol=" << dim("ncol")
      << ", "
      << "nrow=" << dim("nrow")
      << ")";
    return string;
}

QString PlotR::scriptForHistogramPlot(QStringList yLabels, QString geom) const {
    // Parse geom
    int left = geom.indexOf("("),
        right = geom.indexOf(")");
    if (left==-1 || right==-1 || right-left<1)
        ThrowException("Missing bin number in geom").value(geom).
                hint("Write, e.g., histogram(8)").context(this);
    int bins = geom.mid(left+1, right-left).toInt();
    if (bins <= 0)
        bins = 8;
    // Build script
    QString string;
    QTextStream s(&string);
    s << "    "
      << "plot_histogram("
      << df(maxData)
      << ", "
      << "c(" << yLabels.join(", ") << ")"
      << ", "
      << "bins=" << bins
      << ", "
      << "ncol=" << dim("ncol")
      << ", "
      << "nrow=" << dim("nrow")
      << ")";
    return string;
}

QString PlotR::scriptForSobolConvergencePlot() const {
    return "    plot_sobol_convergence()";
}

QString PlotR::scriptForSobolIndicesPlot() {
    QString string;
    QTextStream s(&string);
    s << "    "
      << "plot_sobol_indices";
    if (fontSize > 0)
        s << "(ggplot_theme(" << fontSize << "))";
    else
        s << "()";
    return string;
}

QString PlotR::dim(QString portName) const {
    int value = port(portName)->value<int>();
    return (value == -1) ? QString("NULL") : QString::number(value);
}

void PlotR::appendGgplot(QTextStream &s) {
    if (ggplot.isEmpty())
        return;
    if (!ggplot.simplified().startsWith("+"))
        ggplot.prepend("+");
//    if (!ggplot.contains("geom_"))
//        ggplot.prepend("+" + defaultGgplot);
    if (!guideTitle.isEmpty())
        ggplot.prepend("+labs(colour=\"" + guideTitle + "\")");
    s << ggplot;
}

}
