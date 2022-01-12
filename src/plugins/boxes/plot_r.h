/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PLOT_R_H
#define PLOT_R_H
#include <QString>
#include <QStringList>
#include <QTextStream>
#include "output_ports.h"

namespace base {
    class Path;
    class Port;
}

namespace boxes {

class PlotR : public OutputPorts
{
public:
    PlotR(QString name, Box *parent);
    void reset();
    void initialize();
//    QString toString();
    QString toScript();
private:
    // Inputs
    bool hide;
    QString layout, guideTitle, end, endCode, type, ggplot, transform;
    int maxData, ncol, nrow, iteration, fontSize;
    double width, height;
    base::Path xAxis;
    // Outputs
    bool plotAsList;
    // Methods
    QString dim(QString portName) const;
    void appendGgplot(QTextStream &s);
    QString scriptForDefaultPlot(QStringList xLabels, QStringList yLabels, QString iterationLabel) const;
    QString scriptForDensityPlot(QStringList yLabels) const;
    QString scriptForHistogramPlot(QStringList yLabels, QString type) const;
    QString scriptForSobolConvergencePlot() const;
    QString scriptForSobolIndicesPlot();
};

}

#endif
