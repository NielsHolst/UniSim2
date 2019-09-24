/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PLOT_R_H
#define PLOT_R_H
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <base/track.h>
#include "output_ports.h"
#include "output_r.h"

namespace base {
    class Port;
}

namespace boxes {

class PlotR : public OutputPorts
{
public:
    PlotR(QString name, QObject *parent);
    void reset();
    void initialize();
    QString toString();
    QString toScript();
private:
    // Inputs
    bool hide, showDistributions;
    QVector<QString> ports;
    QString layout, guideTitle, end, endCode, type, ggplot, transform;
    int ncol, nrow, iteration, fontSize;
    double width, height;
    // Outputs
    bool plotAsList;
    // Methods
    QVector<base::Track*> xAxisTracks();
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
